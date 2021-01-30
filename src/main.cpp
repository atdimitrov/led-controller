#include <Arduino.h>
#include <WiFi.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include "config.h"
#include "effects.h"

AsyncWebServer server(80);
LedPanel ledPanel(17, 10, 3);
Effect* currentEffect;

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

bool tryApplyEffect(const JsonVariant &json)
{
    StaticColor *newEffect;
    switch (json["effect"].as<uint8_t>())
    {
        case 0:
        {
            newEffect = new StaticColor(ledPanel, CRGB::Black);
            break;
        }
        case 1:
        {
            uint8_t r = json["staticColor"]["r"];
            uint8_t g = json["staticColor"]["g"];
            uint8_t b = json["staticColor"]["b"];
            newEffect = new StaticColor(ledPanel, CRGB(r, g, b));
            break;
        }
        default:
            return false;
    }
    
    delete currentEffect;
    currentEffect = newEffect;
    return true;
}

void runWebServer()
{
    AsyncCallbackJsonWebHandler* effectHandler = new AsyncCallbackJsonWebHandler("/effect", [](AsyncWebServerRequest *request, JsonVariant &json)
    {
        if (tryApplyEffect(json))
        {
            writeEffectConfig(json);
            request->send(200);
        }
        else
        {
            request->send(400);
        }
    });

    server.addHandler(effectHandler);
    server.begin();
}

void setup()
{
    Serial.begin(115200);

    WiFiConfig wifiConfig = readWifiConfig();
    connectToWiFi(wifiConfig);

    runWebServer();

    StaticJsonDocument<128> json = readEffectConfig();
    if (!tryApplyEffect(json.as<JsonVariant>()))
    {
        Serial.println("Persisted effect config cannot be applied.");
    }
}

void loop()
{
    currentEffect->run();

    FastLED.show();

    delay(1000);
}