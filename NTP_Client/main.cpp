#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstring>
#include <ctime>
#include <sstream>
#include <algorithm>

constexpr unsigned long long NTP_TIMESTAMP_DELTA = 2208988800ull;

struct NTPPacket {
    uint8_t li_vn_mode = 0x1B;
    uint8_t stratum = 0;
    uint8_t poll = 0;
    uint8_t precision = 0;
    uint32_t rootDelay = 0;
    uint32_t rootDispersion = 0;
    uint32_t refId = 0;
    uint32_t refTm_s = 0;
    uint32_t refTm_f = 0;
    uint32_t origTm_s = 0;
    uint32_t origTm_f = 0;
    uint32_t rxTm_s = 0;
    uint32_t rxTm_f = 0;
    uint32_t txTm_s = 0;
    uint32_t txTm_f = 0;
};

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool read_config(const std::string& filename, std::string& server, int& interval) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string section;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;
        if (line.front() == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
            continue;
        }
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));

        if ((section.empty() || section == "config") && key == "server") server = value;
        else if ((section.empty() || section == "config") && key == "interval") interval = std::stoi(value);
    }
    return true;
}

bool sync_ntp(const std::string& server) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    sockaddr_in sa{};
    hostent* he = gethostbyname(server.c_str());
    if (!he) {
        std::cerr << "Failed to resolve server: " << server << std::endl;
        return false;
    }
    memcpy(&sa.sin_addr, he->h_addr, he->h_length);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(123);

#ifdef _WIN32
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif

    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }

    NTPPacket packet{};
    packet.li_vn_mode = 0x1B;

    if (sendto(sock, (char*)&packet, sizeof(packet), 0, (sockaddr*)&sa, sizeof(sa)) < 0) {
        std::cerr << "Failed to send NTP packet" << std::endl;
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }

    sockaddr_in src{};
#ifdef _WIN32
    int srclen = sizeof(src);
#else
    socklen_t srclen = sizeof(src);
#endif

    if (recvfrom(sock, (char*)&packet, sizeof(packet), 0, (sockaddr*)&src, &srclen) < 0) {
        std::cerr << "Failed to receive NTP packet" << std::endl;
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return false;
    }

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    time_t txTm = ntohl(packet.txTm_s) - NTP_TIMESTAMP_DELTA;

    std::tm gmt{};
#ifdef _WIN32
    gmtime_s(&gmt, &txTm);
    SYSTEMTIME st;
    st.wYear = gmt.tm_year + 1900;
    st.wMonth = gmt.tm_mon + 1;
    st.wDay = gmt.tm_mday;
    st.wHour = gmt.tm_hour;
    st.wMinute = gmt.tm_min;
    st.wSecond = gmt.tm_sec;
    st.wMilliseconds = 0;
    if (!SetSystemTime(&st)) {
        std::cerr << "Failed to set system time. Run as Administrator." << std::endl;
        return false;
    }
#else
    gmt = *gmtime(&txTm);
    timeval tv;
    tv.tv_sec = txTm;
    tv.tv_usec = 0;
    if (settimeofday(&tv, nullptr) < 0) {
        std::cerr << "Failed to set system time. Run as root." << std::endl;
        return false;
    }
#endif

    std::cout << "Time synchronized successfully: " << std::asctime(&gmt);
    return true;
}

int main() {
    std::string server = "yzynetwork.xyz";
    int interval = 240;
    if (!read_config("config.ini", server, interval)) {
        std::cerr << "Failed to read config. Using default." << std::endl;
    }
    else {
        std::cout << "Config loaded: server=" << server << ", interval=" << interval << std::endl;
    }
    std::cout << "Starting NTP sync with server: " << server << ", interval: " << interval << "s\n";
    while (true) {
        sync_ntp(server);
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
    return 0;
}
