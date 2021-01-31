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

CRGB ReadColorFromJson(const JsonVariant& json)
{
    uint8_t r = json["r"];
    uint8_t g = json["g"];
    uint8_t b = json["b"];

    return CRGB(r, g, b);
}

bool tryApplyEffect(const JsonVariant &json)
{
    Effect *newEffect;
    switch (json["effect"].as<uint8_t>())
    {
        case 0:
        {
            newEffect = new StaticColor(ledPanel, CRGB::Black);
            break;
        }
        case 1:
        {
            newEffect = new StaticColor(ledPanel, ReadColorFromJson(json["staticColor"]));
            break;
        }
        case 2:
        {
            CRGB startColor = ReadColorFromJson(json["staticGradient"]["start"]);
            CRGB endColor = ReadColorFromJson(json["staticGradient"]["end"]);
            newEffect = new StaticGradient(ledPanel, startColor, endColor);
            break;
        }
        case 3:
        {
            CRGB startColor = ReadColorFromJson(json["runningGradient"]["start"]);
            CRGB endColor = ReadColorFromJson(json["runningGradient"]["end"]);
            uint8_t speed = json["runningGradient"]["speed"];
            uint8_t spreadFactor = json["runningGradient"]["spreadFactor"];
            newEffect = new RunningGradient(ledPanel, startColor, endColor, speed, spreadFactor);
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

    StaticJsonDocument<256> json = readEffectConfig();
    if (!tryApplyEffect(json.as<JsonVariant>()))
    {
        Serial.println("Persisted effect config cannot be applied.");
    }
}

void loop()
{
    currentEffect->run();

    FastLED.show();
}