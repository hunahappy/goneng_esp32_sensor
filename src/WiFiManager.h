#pragma once // 파일이 여러 번 포함되는 것을 방지
#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password);
    bool connect(); // WiFi 연결 시도
    bool isConnected() const; // 연결 상태 확인
    void reset();
    bool reconnect();

private:
    const char* _ssid;
    const char* _password;
};

extern WiFiManager wifi;