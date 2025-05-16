#pragma once
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#ifndef PANEL_MANAGER_H
#define PANEL_MANAGER_H

#include <string>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

enum class PanelType {
    OHR, OHL, AFT_OH, MCP, EFIS, COMM, CDU0_KP, CHRNONL
};

struct PanelConfig {
    std::string ip;
    int rxPort;
    int txPort;
    SOCKET listenSocket;
    SOCKET clientSocket;
    sockaddr_in listenAddr;
    sockaddr_in clientAddr;
};

struct PanelData {
    PanelType panel;
    std::string data;
};

class PanelManager {
public:
    PanelManager();
    ~PanelManager();
    void sendPanelUpdate(const PanelData& data);
    void receivePanelInput();

private:
    std::map<PanelType, PanelConfig> panels_;
    SOCKET udpSock_;
    bool isPanelImplemented(PanelType panel) const;
    uint16_t calcCRC16(const char* packet, size_t len);
    void sendUDPPacket(const PanelConfig& config, const char* data);
};

#endif // PANEL_MANAGER_H