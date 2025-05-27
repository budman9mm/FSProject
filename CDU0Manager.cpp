#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#include "CDU0Manager.h"
#include <windows.h>
#include <cstdio>
#include <stdexcept>

CDU0Manager::CDU0Manager(const std::string& ip, int port) : ip_(ip), port_(port), serverSock_(INVALID_SOCKET), clientSock_(INVALID_SOCKET) {
   WSADATA wsaData;
   int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (result != 0) {
       fprintf(stderr, "WSAStartup failed with error: %d\n", result);
       throw std::runtime_error("Failed to initialize Winsock.");
   }
   serverSock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port_);
   inet_pton(AF_INET, ip_.c_str(), &addr.sin_addr);
   bind(serverSock_, (sockaddr*)&addr, sizeof(addr));
   listen(serverSock_, 5);
}

CDU0Manager::~CDU0Manager() {
    closesocket(serverSock_);
    closesocket(clientSock_);
    WSACleanup();
}

void CDU0Manager::acceptClient() {
    clientSock_ = accept(serverSock_, nullptr, nullptr);
}

void CDU0Manager::sendCDU0Update(const PMDG_CDU0_Data& data) {
    if (data.changed) {
        char packet[1011];
        snprintf(packet, sizeof(packet), "#X%s\n", formatCDU0Data(data).c_str());
        sendNonBlockingTCP(clientSock_, packet);
        printf_s("#X%s", formatCDU0Data(data).c_str());
    }
}

void CDU0Manager::reconnect() {
    if (clientSock_ == INVALID_SOCKET) {
        acceptClient();
    }
}

void CDU0Manager::sendNonBlockingTCP(SOCKET sock, const char* data) {
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(sock, &write_fds);
    timeval timeout = { 0, 100000 };
    if (select((int)sock + 1, nullptr, &write_fds, nullptr, &timeout) > 0) {
        send(sock, data, 1011, 0);
    }
}

std::string CDU0Manager::formatCDU0Data(const PMDG_CDU0_Data& data) {
    return std::string(data.data, 1008);
}