#pragma once
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

// Undefine potential macro conflicts early
#undef SConManager
#undef string

#ifndef SCON_MANAGER_H
#define SCON_MANAGER_H

#include <windows.h>
#include <SimConnect.h>
#include <string>
#include "PMDG_NG3_SDK.h"
#include "CDU0Manager.h"
#include "PanelManager.h"

// Forward declare PMDG structures (defined in PMDG SDK)
struct PMDG_NG3_CDU_Screen;
struct PMDG_NG3_Data;

namespace FSProject {
    class SConManager {
    public:
        SConManager(const std::string& appName, CDU0Manager& cdu0, PanelManager& panels);
        ~SConManager();
        bool isConnected() const;
        HANDLE getSimConnectHandle() const;
        static void CALLBACK dispatchCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

    private:
        HANDLE hSimConnect_;
        CDU0Manager& cdu0_;
        PanelManager& panels_;

        // Request IDs (user-defined)
        static const int CDU0_REQUEST = 1;
        static const int PM_DATA_REQUEST = 3;
        static const int PM_CONTROL_REQUEST = 2;

        void setupClientData();
    };
}

#endif // SCON_MANAGER_H