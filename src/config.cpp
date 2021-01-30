#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "config.h"

WiFiConfig::WiFiConfig(const char* ssid, const char* password)
{
    strlcpy(this->ssid, ssid, 20);
    strlcpy(this->password, password, 20);
}

WiFiConfig readWifiConfig()
{
    SPIFFS.begin();
    File configFile = SPIFFS.open("/wifi-config.json", FILE_READ);
    
    StaticJsonDocument<128> jsonDoc;
    deserializeJson(jsonDoc, configFile);
    WiFiConfig config = WiFiConfig(jsonDoc["ssid"], jsonDoc["password"]);

    configFile.close();
    
    return config;
}

StaticJsonDocument<128> readEffectConfig()
{
    SPIFFS.begin();
    File configFile = SPIFFS.open("/effect-config.json", FILE_READ);
    
    DynamicJsonDocument jsonDoc(128);
    deserializeJson(jsonDoc, configFile);

    configFile.close();
    
    return jsonDoc;
}

void writeEffectConfig(StaticJsonDocument<128> jsonDoc)
{
    SPIFFS.begin();
    File configFile = SPIFFS.open("/effect-config.json", FILE_WRITE);
    
    serializeJson(jsonDoc, configFile);

    configFile.close();
}