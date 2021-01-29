#include <Arduino.h>
#include <WiFi.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include "config.h"
#include "effects.h"

AsyncWebServer server(80);

Effect* currentEffect;
LedPanel ledPanel = LedPanel(17, 10, 3);

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

void runWebServer()
{
    AsyncCallbackJsonWebHandler* effectHandler = new AsyncCallbackJsonWebHandler("/effect", [](AsyncWebServerRequest *request, JsonVariant &json)
    {
        switch (json["effect"].as<uint8_t>())
        {
            case 0:
                currentEffect = new StaticColor(ledPanel, json["hue"]);
                break;
            
            default:
                request->send(400);
                return;
        }

        request->send(200);
    });

    server.addHandler(effectHandler);

    server.on("/none", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        request->send(200);
        delete currentEffect;
        currentEffect = nullptr;
    });

    server.begin();
}

void setup()
{
    Serial.begin(115200);

    WiFiConfig wifiConfig = readWifiConfig();
    connectToWiFi(wifiConfig);

    runWebServer();
}

void loop()
{
    if (currentEffect != nullptr)
    {
        currentEffect->run();
    }

    FastLED.show();

    delay(1000);
}