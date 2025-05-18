#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#include "SConManager.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include "PMDG_NG3_SDK.h"

// Undefine potential macro conflicts
#undef SConManager
#undef string
#undef Cells
#undef Symbol
#undef Color
#undef Flags

// Define CDU dimensions from PMDG_NG3_SDK.h
#define CDU_COLUMNS 24
#define CDU_ROWS 14
#define CDU_CELLS (CDU_COLUMNS * CDU_ROWS)

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
    hr = SimConnect_MapClientDataNameToID(hSimConnect_, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);
    hr = SimConnect_AddToClientDataDefinition(hSimConnect_, PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);
    hr = SimConnect_RequestClientData(hSimConnect_, PMDG_NG3_CDU_0_ID, CDU0_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
        SIMCONNECT_CLIENT_DATA_PERIOD_VISUAL_FRAME, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
    hr = SimConnect_MapClientDataNameToID(hSimConnect_, PMDG_NG3_DATA_NAME, PMDG_NG3_DATA_ID);
    hr = SimConnect_AddToClientDataDefinition(hSimConnect_, PMDG_NG3_DATA_DEFINITION, 0, sizeof(PMDG_NG3_Data), 0, 0);
    hr = SimConnect_RequestClientData(hSimConnect_, PMDG_NG3_DATA_ID, PM_DATA_REQUEST, PMDG_NG3_DATA_DEFINITION,
        SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
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
            // Build ASCII string packet without #X (assume sendCDU0Update adds it)
            std::string fullPackage = "";
            for (int h = 0; h < 3; h++) {
                for (int i = 0; i < CDU_ROWS; i++) {
                    for (int j = 0; j < CDU_COLUMNS; j++) {
                        if (h == 0) {
                            char s = static_cast<char>(cduData->Cells[j][i].Symbol);
                            fullPackage += s; // Symbol as char
                        }
                        else {
                            char v = (h == 1) ? static_cast<char>(cduData->Cells[j][i].Color) : static_cast<char>(cduData->Cells[j][i].Flags);
                            fullPackage += std::to_string(static_cast<int>(v)); // Color/Flag as decimal string
                        }
                    }
                }
            }
            // Copy to PMDG_CDU0_Data for sendCDU0Update
            PMDG_CDU0_Data cdu0Data;
            cdu0Data.changed = cduData->Powered;
            size_t len = fullPackage.length() < sizeof(cdu0Data.data) ? fullPackage.length() : sizeof(cdu0Data.data);
            std::memcpy(cdu0Data.data, fullPackage.c_str(), len);
            if (len < sizeof(cdu0Data.data)) {
                std::memset(cdu0Data.data + len, 0, sizeof(cdu0Data.data) - len);
            }

            // Debug: Raw symbol grid (hex)
            std::cout << "CDU0 Symbol Grid (24x14, raw hex):\n";
            for (int row = 0; row < CDU_ROWS; ++row) {
                for (int col = 0; col < CDU_COLUMNS; ++col) {
                    unsigned char symbol = static_cast<unsigned char>(cduData->Cells[col][row].Symbol);
                    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(symbol) << " ";
                }
                std::cout << std::dec << '\n';
            }
            std::cout << std::endl;

            // Debug: Color grid
            std::cout << "CDU0 Color Grid (24x14, 0-5):\n";
            for (int row = 0; row < CDU_ROWS; ++row) {
                for (int col = 0; col < CDU_COLUMNS; ++col) {
                    int color = static_cast<int>(cduData->Cells[col][row].Color);
                    std::cout << std::setw(4) << color;
                }
                std::cout << '\n';
            }
            std::cout << std::endl;

            // Debug: Flags grid
            std::cout << "CDU0 Flags Grid (24x14, 0-4):\n";
            for (int row = 0; row < CDU_ROWS; ++row) {
                for (int col = 0; col < CDU_COLUMNS; ++col) {
                    int flags = static_cast<int>(cduData->Cells[col][row].Flags);
                    std::cout << std::setw(4) << flags;
                }
                std::cout << '\n';
            }
            std::cout << std::endl;

            // Debug: Packet contents (ASCII)
            std::cout << "CDU0 Packet Contents (ASCII, " << fullPackage.length() << " bytes, #X added by sendCDU0Update):\n";
            std::string debugPackage = "#X" + fullPackage; // For debug display
            for (size_t i = 0; i < debugPackage.length(); ++i) {
                char c = debugPackage[i];
                if (c < 32 || c > 126) c = '.';
                std::cout << c;
                if ((i + 1) % 64 == 0) std::cout << '\n';
            }
            std::cout << std::endl;

            // Debug: Packet contents (hex)
            std::cout << "CDU0 Packet Contents (hex, with #X):\n";
            for (size_t i = 0; i < debugPackage.length(); ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(static_cast<unsigned char>(debugPackage[i]))) << " ";
                if ((i + 1) % 16 == 0) std::cout << '\n';
            }
            std::cout << std::dec << std::endl;

            std::cout << "Sending CDU0 update, size: " << fullPackage.length() << " bytes (plus #X)\n";
            manager->cdu0_.sendCDU0Update(cdu0Data);
        }
    }
    break;
    default:
        break;
    }
}