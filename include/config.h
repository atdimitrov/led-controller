#ifndef CONFIG_H
#define CONFIG_H

struct WiFiConfig
{
    public:
        char ssid[20];
        char password[20];

        WiFiConfig(const char* ssid, const char* password);
};

WiFiConfig readWifiConfig();

StaticJsonDocument<256> readEffectConfig();

void writeEffectConfig(StaticJsonDocument<256> jsonDoc);

#endif