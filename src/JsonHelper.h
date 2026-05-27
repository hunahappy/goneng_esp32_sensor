#include <ArduinoJson.h>

String createJson(const char* key, const char* value)
{
    StaticJsonDocument<128> doc;

    doc[key] = value;

    String output;
    serializeJson(doc, output);

    return output;
}

template<typename T>
String createJson(const char* key, T value)
{
    StaticJsonDocument<128> doc;

    doc[key] = value;

    String output;
    serializeJson(doc, output);

    return output;
}