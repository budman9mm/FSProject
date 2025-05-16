#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#include "PanelManager.h"
#include "CDU0Manager.h"
#include "SConManager.h"
#include <windows.h>
#include <string>
#include <regex>
#include <iostream>

std::string parseServerIP(int argc, char* argv[]) {
    std::string defaultIP = "192.168.1.5";
    std::regex ipRegex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--ip" && i + 1 < argc) {
            std::string ip = argv[i + 1];
            if (std::regex_match(ip, ipRegex)) {
                return ip;
            }
            else {
                std::cerr << "Invalid IP format: " << ip << ". Using default: " << defaultIP << std::endl;
                return defaultIP;
            }
        }
    }
    return defaultIP;
}

int main(int argc, char* argv[]) {
    std::string serverIP = parseServerIP(argc, argv);
    std::cout << "Using server IP: " << serverIP << std::endl;

    CDU0Manager cdu0(serverIP, 27015);
    PanelManager panels;
    FSProject::SConManager simConnect("PMDG 737 Controller", cdu0, panels);

    if (!simConnect.isConnected()) {
        std::cerr << "SimConnect initialization failed" << std::endl;
        return 1;
    }

    cdu0.acceptClient();

    while (true) {
        SimConnect_CallDispatch(simConnect.getSimConnectHandle(), [](SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
            FSProject::SConManager::dispatchCallback(pData, cbData, pContext);
            }, &simConnect);

        panels.receivePanelInput();
        cdu0.reconnect();
        Sleep(100);
    }

    return 0;
}