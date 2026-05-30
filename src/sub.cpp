#include "WiFiManager.h"
#include "Logger.h"
#include "MqttManager.h"
#include "JsonHelper.h"
#include "sub.h"    

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <BH1750.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SDA1 17
#define SCL1 18
Adafruit_AHTX0 aht1;
BH1750 lightMeter;

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define EC_PIN 5

#define CALIBRATION_FACTOR 1.21
// #define RELAY_PIN 16

void check_wifi_and_start() {
    if (WiFi.status() != WL_CONNECTED) {
        logger.log("warn", "WiFi 연결 시도...");
        wifi.reconnect();
    }
}   

unsigned long lastReconnectAttempt = 0;
void check_mqtt_and_start(const char* check_connect) {
    if (WiFi.status() == WL_CONNECTED) {
        if (!mqtt.isConnected()) {
            unsigned long now = millis();
            // 5초마다 한 번씩만 재연결 시도
            if (check_connect=="시작" || (now - lastReconnectAttempt > 5000)) {
                lastReconnectAttempt = now;
                logger.log("warn", "MQTT 연결 시도...");
                if (mqtt.connect()) {
                    lastReconnectAttempt = 0; // 성공하면 타이머 초기화
                    const char* str_data = R"({
                        "장치": "esp32_sensor",
                        "구분": "log",
                        "내용":{
                            "메시지": "mqtt 연결 성공!"
                        }
                    })";
                    mqtt.publish("goneng/farm1/log/sensor/status", str_data);
                }
            }
        } else {
            // MQTT 연결 중일 때만 루프 실행
            mqtt.loop();
        }
    }   
}

void init_sensor() {
    Wire.begin(SDA1, SCL1, 100000);

    if (!aht1.begin()) {
        const char* str_data = R"({
                "장치": "esp32_sensor",
                "구분": "log",
                내용:{
                    "메시지": "sensor 시작!"
                }
            }
        )";
        mqtt.publish("goneng/farm1/log/sensor/status", str_data);
    } else {
        const char* str_data = R"({
                "장치": "esp32_sensor",
                "구분": "log",
                "내용":{
                    "메시지": "온습도 센서 초기화 완료!"
                }
            }
        )";
        mqtt.publish("goneng/farm1/log/sensor/status", str_data);
    }

    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        const char* str_data = R"({
                "장치": "esp32_sensor",
                "구분": "log",
                "내용":{
                    "메시지": "조도 센서 초기화 성공!"
                }
            }
        )";
        mqtt.publish("goneng/farm1/log/sensor/status", str_data);
    } else {
        const char* str_data = R"({
                "장치": "esp32_sensor",
                "구분": "log",
                "내용":{
                    "메시지": "조도 센서 초기화 실패!"
                }
            }
        )";
        mqtt.publish("goneng/farm1/log/sensor/status", str_data);
    }    

    sensors.begin();

    analogReadResolution(12);
    analogSetPinAttenuation(EC_PIN, ADC_11db);

    // pinMode(RELAY_PIN, OUTPUT);
    // digitalWrite(RELAY_PIN, HIGH);    
}   

void read_sensor_and_publish() {    
    sensors_event_t h1, t1;    
    aht1.getEvent(&h1, &t1);

    JsonDocument doc; 
    doc["장치"] = "sensor";
    doc["구분"] = "data";

    JsonObject content = doc.createNestedObject("내용");

    content["온도"] = t1.temperature;
    content["습도"] = h1.relative_humidity;

    //////////////////////////////////////////////////////////////////////////

    float lux = lightMeter.readLightLevel();
    content["조도"] = lux;    

    /////////////////////////////////////////////////////////////////////////

    sensors.requestTemperatures(); 
    float tempC = sensors.getTempCByIndex(0);
    content["수온"] = tempC;

    /////////////////////////////////////////////////////////////////////////

    int sum = 0;
    for(int i = 0; i < 20; i++) { // 20번 읽어서 평균 내기
        sum += analogRead(EC_PIN);
        delay(5); // 센서 안정화를 위한 짧은 딜레이
    }
    float avgRaw = sum / 20.0;
    
    float voltage = avgRaw * 3.3 / 4095.0;
    float ec = voltage * CALIBRATION_FACTOR;
    float ec25 = ec / (1 + 0.02 * (tempC - 25));
    content["EC"] = ec25;

    String jsonOutput;
    serializeJson(doc, jsonOutput);
    mqtt.publish("goneng/farm1/data/sensor/thes", jsonOutput.c_str());    
}    





    // digitalWrite(RELAY_PIN, LOW); //작동
    // delay(1000);  
    // digitalWrite(RELAY_PIN, HIGH); //대기
    // delay(1000);
