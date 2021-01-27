#include <Arduino.h>
#include <WiFi.h>
#include <AsyncWebSocket.h>
#include "config.h"

AsyncWebServer server(80);

void connectToWiFi(WiFiConfig wifiConfig)
{
    Serial.printf("Connecting to WiFi network %s...\n", wifiConfig.ssid);
    
    WiFi.begin(wifiConfig.ssid, wifiConfig.password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("Connected");
}

void setup()
{
    Serial.begin(115200);

    WiFiConfig wifiConfig = readWifiConfig();
    connectToWiFi(wifiConfig);

    server.on("/static-color", HTTP_POST, [](AsyncWebServerRequest *request){
        request->send(200);
        Serial.println("POST /static-color received.");
    });

    server.on("/none", HTTP_POST, [](AsyncWebServerRequest *request){
        request->send(200);
        Serial.println("POST /none received.");
    });

    server.begin();
}

void loop()
{
}