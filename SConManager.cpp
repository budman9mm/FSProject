#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#include "SConManager.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include "PMDG_NG3_SDK.h"

// Undefine potential macro conflicts
#undef SConManager
#undef string

// Define CDU dimensions from PMDG_NG3_SDK.h
#define CDU_COLUMNS 24
#define CDU_ROWS 14

FSProject::SConManager::SConManager(const std::string& appName, CDU0Manager& cdu0, PanelManager& panels)
    : hSimConnect_(nullptr), cdu0_(cdu0), panels_(panels) {
    if (SimConnect_Open(&hSimConnect_, appName.c_str(), nullptr, 0, 0, 0) != S_OK) {
        std::cerr << "Failed to connect to SimConnect" << std::endl;
        return;
    }
    setupClientData();
    SimConnect_CallDispatch(hSimConnect_, dispatchCallback, this);
}

FSProject::SConManager::~SConManager() {
    if (hSimConnect_) {
        SimConnect_Close(hSimConnect_);
    }
}

bool FSProject::SConManager::isConnected() const {
    return hSimConnect_ != nullptr;
}

HANDLE FSProject::SConManager::getSimConnectHandle() const {
    return hSimConnect_;
}

void FSProject::SConManager::setupClientData() {
    HRESULT hr;
    // CDU Data
    hr = SimConnect_MapClientDataNameToID(hSimConnect_, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);
    hr = SimConnect_AddToClientDataDefinition(hSimConnect_, PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);
    hr = SimConnect_RequestClientData(hSimConnect_, PMDG_NG3_CDU_0_ID, CDU0_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
        SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
    // PMDG Control Data
    hr = SimConnect_MapClientDataNameToID(hSimConnect_, PMDG_NG3_DATA_NAME, PMDG_NG3_DATA_ID);
    hr = SimConnect_AddToClientDataDefinition(hSimConnect_, PMDG_NG3_DATA_DEFINITION, 0, sizeof(PMDG_NG3_Data), 0, 0);
    hr = SimConnect_RequestClientData(hSimConnect_, PMDG_NG3_DATA_ID, PM_DATA_REQUEST, PMDG_NG3_DATA_DEFINITION,
        SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
    // PMDG Control Change
    hr = SimConnect_RequestClientData(hSimConnect_, PMDG_NG3_CONTROL_ID, PM_CONTROL_REQUEST, PMDG_NG3_CONTROL_DEFINITION,
        SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
}

void FSProject::SConManager::dispatchCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
    FSProject::SConManager* manager = static_cast<FSProject::SConManager*>(pContext);
    switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_CLIENT_DATA:
    {
        SIMCONNECT_RECV_CLIENT_DATA* clientData = static_cast<SIMCONNECT_RECV_CLIENT_DATA*>(pData);
        if (clientData->dwRequestID == CDU0_REQUEST) {
            PMDG_NG3_CDU_Screen* cduData = reinterpret_cast<PMDG_NG3_CDU_Screen*>(&clientData->dwData);
            PMDG_CDU0_Data cdu0Data;
            cdu0Data.changed = cduData->Powered;
            int idx = 0;
            // Debug raw Symbol values for first row
            std::cout << "Raw Symbol values (first row):\n";
            for (int col = 0; col < CDU_COLUMNS; ++col) {
                char c = cduData->Cells[col][0].Symbol;
                std::cout << "Col " << col << ": " << (int)c << " (0x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c << std::dec << ")\n";
            }
            for (int col = 0; col < CDU_COLUMNS; ++col) {
                for (int row = 0; row < CDU_ROWS; ++row) {
                    char symbol = cduData->Cells[col][row].Symbol;
                    // Clamp Symbol to ASCII or known specials
                    if (symbol != -95 && symbol != -94 && symbol != -80 && symbol != -22 &&
                        (symbol < 32 || symbol > 126)) {
                        symbol = ' '; // Replace invalid with space
                    }
                    cdu0Data.data[idx++] = symbol;
                    cdu0Data.data[idx++] = cduData->Cells[col][row].Color; // 0-5
                    cdu0Data.data[idx++] = cduData->Cells[col][row].Flags; // 0-4
                }
            }
            // Log cdu0Data.data as 24x14 grid
            std::cout << "CDU0 Data (24x14):\n";
            for (int row = 0; row < CDU_ROWS; ++row) {
                for (int col = 0; col < CDU_COLUMNS; ++col) {
                    int base_idx = (col * CDU_ROWS + row) * 3;
                    char c = cdu0Data.data[base_idx];
                    if (c == -95) c = 'a'; // Arrow
                    if (c == -94) c = 'b'; // Arrow
                    if (c == -80) c = 'c'; // Degree
                    if (c == -22) c = 'd'; // Box
                    if (c < 32 || c > 126) c = '.'; // Non-printable to dot
                    std::cout << c;
                }
                std::cout << '\n';
            }
            std::cout << std::endl;
            std::cout << "Sending CDU0 update, size: " << sizeof(cdu0Data.data) << " bytes\n";
            manager->cdu0_.sendCDU0Update(cdu0Data);
        }
        // TODO: Handle PM_DATA_REQUEST and PM_CONTROL_REQUEST
    }
    break;
    default:
        break;
    }
}