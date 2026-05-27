#include "WiFiManager.h"
#include "Logger.h"
#include "MqttManager.h"
#include "sub.h"

Logger logger(LogLevel::INFO);
WiFiManager wifi("iptime", "00000000");
MqttManager mqtt("192.168.0.121", 1883, "goneng", "goneng11!");

void setup() {
    Serial.begin(115200);

    check_wifi_and_start();
    check_mqtt_and_start("시작");

    init_sensor();
    mqtt.publish("goneng/farm1/log/sensor_control_1/status", "{\"센서\": \"sensor_control_1\", \"로그\": \"sensor_control_1 시작!\"}");
}


unsigned long lastSensorTime = 0;
const unsigned long SENSOR_INTERVAL = 10000; // 10초

void loop() {
    check_wifi_and_start();
    check_mqtt_and_start("루프");

    unsigned long now = millis();
    if (now - lastSensorTime >= SENSOR_INTERVAL) {
        lastSensorTime = now;
        read_sensor_and_publish();
    }
}