#pragma once
#include <PubSubClient.h>
#include <WiFi.h>

class MqttManager {
public:
    MqttManager(const char* server, uint16_t port, const char* user, const char* pass);
    
    bool connect(); // 인증 정보를 사용한 연결
    void loop();
    bool isConnected();
    bool publish(const char* topic, const char* payload);

private:
    const char* _server;
    uint16_t _port;
    const char* _user;
    const char* _pass;
    WiFiClient _wifiClient;
    PubSubClient _mqttClient;
};

extern MqttManager mqtt; // 다른 파일에서 참조 가능