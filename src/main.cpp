#include <Arduino.h>
#include <WiFi.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include "config.h"
#include "effects.h"

AsyncWebServer server(80);

LedPanel ledPanel(27, 10, 27);
LedPanel deskStrip(32, 90, 1);
LedPanel shelfStrip(19, 65, 1);

Effect* ledPanelEffect;
Effect* deskStripEffect;
Effect* shelfStripEffect;

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

CHSV ReadColorFromJson(const JsonVariant& json)
{
    uint8_t h = json["h"];
    uint8_t s = json["s"];
    uint8_t v = json["v"];

    return CHSV(h, s, v);
}

bool tryApplyEffect(const JsonVariant &json)
{
    Effect *newLedPanelEffect;
    Effect *newDeskStripEffect;
    Effect *newShelfStripEffect;

    switch (json["effect"].as<uint8_t>())
    {
        case 0:
        {
            newLedPanelEffect = new StaticColor(ledPanel, CHSV(0, 0, 0));
            newDeskStripEffect = new StaticColor(deskStrip, CHSV(0, 0, 0));
            newShelfStripEffect = new StaticColor(shelfStrip, CHSV(0, 0, 0));

            break;
        }
        case 1:
        {
            CHSV color = ReadColorFromJson(json["staticColor"]);

            newLedPanelEffect = new StaticColor(ledPanel, color);
            newDeskStripEffect = new StaticColor(deskStrip, color);
            newShelfStripEffect = new StaticColor(shelfStrip, color);

            break;
        }
        case 2:
        {
            CHSV startColor = ReadColorFromJson(json["staticGradient"]["start"]);
            CHSV endColor = ReadColorFromJson(json["staticGradient"]["end"]);
            bool reverse = json["staticGradient"]["reverse"];
            TGradientDirectionCode direction = json["staticGradient"]["direction"];

            newLedPanelEffect = new StaticGradient(ledPanel, startColor, endColor, reverse, direction);
            newDeskStripEffect = new StaticGradient(deskStrip, startColor, endColor, reverse, direction);
            newShelfStripEffect = new StaticGradient(shelfStrip, startColor, endColor, reverse, direction);

            break;
        }
        case 3:
        {
            CHSV startColor = ReadColorFromJson(json["runningGradient"]["start"]);
            CHSV endColor = ReadColorFromJson(json["runningGradient"]["end"]);
            uint8_t speed = json["runningGradient"]["speed"];
            uint8_t spreadFactor = json["runningGradient"]["spreadFactor"];
            bool reverse = json["runningGradient"]["reverse"];
            TGradientDirectionCode direction = json["runningGradient"]["direction"];

            newLedPanelEffect = new RunningGradient(ledPanel, startColor, endColor, speed, spreadFactor, reverse, direction);
            newDeskStripEffect = new RunningGradient(deskStrip, startColor, endColor, speed, spreadFactor, reverse, direction);
            newShelfStripEffect = new RunningGradient(shelfStrip, startColor, endColor, speed, spreadFactor, reverse, direction);

            break;
        }
        case 4:
        {
            CHSV primaryColor = ReadColorFromJson(json["starlight"]["primaryColor"]);
            CHSV secondaryColor = ReadColorFromJson(json["starlight"]["secondaryColor"]);
            uint8_t speed = json["starlight"]["speed"];

            newLedPanelEffect = new Starlight(ledPanel, primaryColor, secondaryColor, speed);
            newDeskStripEffect = new Starlight(deskStrip, primaryColor, secondaryColor, speed);
            newShelfStripEffect = new Starlight(shelfStrip, primaryColor, secondaryColor, speed);

            break;
        }
        case 5:
        {
            CHSV color1 = ReadColorFromJson(json["paletteRandomness"]["color1"]);
            CHSV color2 = ReadColorFromJson(json["paletteRandomness"]["color2"]);
            CHSV color3 = ReadColorFromJson(json["paletteRandomness"]["color3"]);
            CHSV color4 = ReadColorFromJson(json["paletteRandomness"]["color4"]);
            uint8_t speed = json["paletteRandomness"]["speed"];

            newLedPanelEffect = new PaletteRandomness(ledPanel, color1, color2, color3, color4, speed);
            newDeskStripEffect = new PaletteRandomness(deskStrip, color1, color2, color3, color4, speed);
            newShelfStripEffect = new PaletteRandomness(shelfStrip, color1, color2, color3, color4, speed);

            break;
        }
        default:
            return false;
    }
    
    delete ledPanelEffect;
    delete deskStripEffect;
    delete shelfStripEffect;

    ledPanelEffect = newLedPanelEffect;
    deskStripEffect = newDeskStripEffect;
    shelfStripEffect = newShelfStripEffect;

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
    ledPanelEffect->run();
    deskStripEffect->run();
    shelfStripEffect->run();

    FastLED.show();
}