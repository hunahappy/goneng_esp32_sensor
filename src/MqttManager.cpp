#include "MqttManager.h"
#include "Logger.h"

// 1. 메시지 수신 시 실행될 정적 함수 (라이브러리 제약상 static 또는 전역이어야 함)
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // payload를 문자열로 안전하게 변환
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = '\0';

    // 로그 출력
    Serial.printf("메시지 도착! 토픽: %s, 내용: %s\n", topic, msg);

    // 2. 토픽에 따른 로직 분기
    if (String(topic) == "gonng/control/sensor_control_1/action1") {
        if (String(msg) == "ON") {
            // LED 켜는 로직
        }
    }
}

MqttManager::MqttManager(const char* server, uint16_t port, const char* user, const char* pass) 
    : _server(server), _port(port), _user(user), _pass(pass), _mqttClient(_wifiClient) {
    _mqttClient.setServer(_server, _port);
    _mqttClient.setCallback(mqttCallback); 
}

bool MqttManager::connect() {
    // connect(clientId, user, pass) 사용
    if (_mqttClient.connect("WasabiFarm_S3", _user, _pass)) {
        _mqttClient.subscribe("goneng/farm1/control/sensor_control_1/#");
        logger.log("info", "MQTT Authentication 성공.");
        return true;
    } else {
        logger.log("err", "MQTT Authentication 실패.");
        return false;
    }
    return false;
}

void MqttManager::loop() {
    if (_mqttClient.connected()) {
        _mqttClient.loop();
    }
}

bool MqttManager::isConnected(){
    return _mqttClient.connected();
}

bool MqttManager::publish(const char* topic, const char* payload) {
    if (!isConnected()) {
        logger.log("err", "mqtt 연결 안됨.");
        return false; // 연결 안 됨
    }
    // _mqttClient.publish는 성공 시 true 반환
    logger.log("info", payload);
    return _mqttClient.publish(topic, payload);
}


