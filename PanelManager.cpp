#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4996) // Suppress CRT secure warnings
#pragma warning(disable: 28182) // Suppress Winsock deprecated warnings

#include "PanelManager.h"
#include <windows.h>
#include <cstdio>

PanelManager::PanelManager() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    udpSock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    panels_[PanelType::OHR] = { "192.168.1.130", 47029, 47028, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::OHL] = { "192.168.1.120", 47033, 47032, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::AFT_OH] = { "192.168.1.121", 47021, 47020, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::MCP] = { "192.168.1.140", 47023, 47022, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::EFIS] = { "192.168.1.144", 47025, 47024, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::COMM] = { "192.168.1.154", 47027, 47026, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::CDU0_KP] = { "192.168.1.134", 47035, 47034, INVALID_SOCKET, INVALID_SOCKET, {}, {} };
    panels_[PanelType::CHRNONL] = { "192.168.1.123", 47037, 47036, INVALID_SOCKET, INVALID_SOCKET, {}, {} };

    for (auto it = panels_.begin(); it != panels_.end(); ++it) {
        it->second.listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        it->second.listenAddr.sin_family = AF_INET;
        it->second.listenAddr.sin_port = htons(it->second.rxPort);
        inet_pton(AF_INET, "0.0.0.0", &it->second.listenAddr.sin_addr);
        bind(it->second.listenSocket, (sockaddr*)&it->second.listenAddr, sizeof(it->second.listenAddr));
    }
}

PanelManager::~PanelManager() {
    for (auto it = panels_.begin(); it != panels_.end(); ++it) {
        closesocket(it->second.listenSocket);
        closesocket(it->second.clientSocket);
    }
    closesocket(udpSock_);
    WSACleanup();
}

void PanelManager::sendPanelUpdate(const PanelData& data) {
    if (!isPanelImplemented(data.panel)) return;
    auto it = panels_.find(data.panel);
    if (it != panels_.end()) {
        sendUDPPacket(it->second, data.data.c_str());
    }
}

void PanelManager::receivePanelInput() {
    for (auto it = panels_.begin(); it != panels_.end(); ++it) {
        if (!isPanelImplemented(it->first)) continue;
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(it->second.listenSocket, &read_fds);
        timeval timeout = { 0, 100000 };
        if (select((int)it->second.listenSocket + 1, &read_fds, nullptr, nullptr, &timeout) > 0) {
            char buffer[512];
            int addrSize = sizeof(it->second.clientAddr);
            int bytes = recvfrom(it->second.listenSocket, buffer, sizeof(buffer), 0, (sockaddr*)&it->second.clientAddr, &addrSize);
            if (bytes > 0) {
                buffer[bytes] = '\0';
                char* context = nullptr;
                char* data = strtok_s(buffer, "|", &context);
                char* checksumStr = strtok_s(nullptr, "|", &context);
                if (data && checksumStr) {
                    uint16_t receivedChecksum = strtol(checksumStr, nullptr, 16);
                    if (calcCRC16(data, strlen(data)) == receivedChecksum) {
                        // Process input
                    }
                }
            }
        }
    }
}

bool PanelManager::isPanelImplemented(PanelType panel) const {
    return panel != PanelType::AFT_OH;
}

uint16_t PanelManager::calcCRC16(const char* packet, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)packet[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (crc & 1 ? 0xA001 : 0);
        }
    }
    return crc;
}

void PanelManager::sendUDPPacket(const PanelConfig& config, const char* data) {
    char packet[512];
    uint16_t checksum = calcCRC16(data, strlen(data));
    snprintf(packet, sizeof(packet), "%s|%04X", data, checksum);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config.txPort);
    inet_pton(AF_INET, config.ip.c_str(), &addr.sin_addr);
    sendto(udpSock_, packet, strlen(packet), 0, (sockaddr*)&addr, sizeof(addr));
}