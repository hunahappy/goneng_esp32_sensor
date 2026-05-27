#include "WiFiManager.h"
#include "Logger.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {} // 멤버 초기화 리스트

bool WiFiManager::connect() {
    Serial.printf("연결 %s...", _ssid);

    WiFi.mode(WIFI_STA);
    WiFi.persistent(false);
    WiFi.setAutoReconnect(false);
    WiFi.setSleep(false);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    
    WiFi.begin(_ssid, _password);
    
    // 타임아웃 처리를 포함한 연결 로직
    uint8_t timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 120) {
        delay(1000);
        Serial.printf(".");
        timeout++;
    }

    if (isConnected()) {
        logger.log("info", "wifi 연결 성공.");        
        logger.log("info", (String("WiFi RSSI: ") + WiFi.RSSI()).c_str());
        logger.log("info", (String("IP Address: ") + WiFi.localIP().toString()).c_str());
        return true;
    } else {
        logger.log("err", "wifi 연결 실패.");
        return false;
    }
    return false;
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reset()
{
    logger.log("warn", "WiFi reset");
    WiFi.disconnect(true, true);
    delay(1000);
    WiFi.mode(WIFI_OFF);
    delay(1000);
    WiFi.mode(WIFI_STA);
}

bool WiFiManager::reconnect()
{
    logger.log("warn", "WiFi reconnecting...");
    reset();
    return connect();
}