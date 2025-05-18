#pragma once
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#ifndef CDU0_MANAGER_H
#define CDU0_MANAGER_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

struct PMDG_CDU0_Data {
    bool changed;
    char data[1008]; // 24x14 symbols (assuming only Symbol from PMDG_NG3_CDU_Cell)
};

class CDU0Manager {
public:
    CDU0Manager(const std::string& ip, int port);
    ~CDU0Manager();
    void acceptClient();
    void sendCDU0Update(const PMDG_CDU0_Data& data);
    void reconnect();

private:
    std::string ip_;
    int port_;
    SOCKET serverSock_;
    SOCKET clientSock_;
    void sendNonBlockingTCP(SOCKET sock, const char* data);
    std::string formatCDU0Data(const PMDG_CDU0_Data& data);
};

#endif // CDU0_MANAGER_H