#ifndef LED_PANEL_H
#define LED_PANEL_H

#include <Arduino.h>
#include <FastLED.h>
#include "led-section.h"

struct LedPanel
{
    public:
        LedPanel(uint8_t pin, uint8_t numberOfSections, uint numberOfPixelsPerSection);
        ~LedPanel();
        const uint8_t numberOfSections;
        LedSection** sections;
    private:
        const uint8_t numberOfPixelsPerSection;
        const uint8_t numberOfPixels;
        CRGB* pixels;
};

#endif