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

    const char* str_data = R"({
            "장치": "esp32_sensor",
            "구분": "log",
            "내용":{
                "메시지": "sensor 시작!"
            }
        }
    )";
    mqtt.publish("goneng/farm1/log/sensor/status", str_data);
}

unsigned long lastSensorTime = 0;
const unsigned long SENSOR_INTERVAL = 40000; // 40초

void loop() {
    check_wifi_and_start();
    check_mqtt_and_start("루프");

    unsigned long now = millis();
    if (now - lastSensorTime >= SENSOR_INTERVAL) {
        lastSensorTime = now;
        read_sensor_and_publish();
    }
}