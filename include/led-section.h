#ifndef LED_SECTION_H
#define LED_SECTION_H

#include <FastLED.h>

struct LedSection
{
    public:
        LedSection(uint8_t numberOfPixels, CRGB* pixels);
        void setColor(uint8_t hue);
        void setColor(CHSV hsv);
    private:
        const uint8_t numberOfPixels;
        CRGB* pixels;
};

#endif