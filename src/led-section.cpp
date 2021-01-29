#include <FastLED.h>
#include "led-section.h"

LedSection::LedSection(uint8_t numberOfPixels, CRGB *pixels)
    : numberOfPixels(numberOfPixels),
      pixels(pixels)
{
}

void LedSection::setColor(uint8_t hue)
{
    setColor(CHSV(hue, 255, 150));
}

void LedSection::setColor(CHSV chsv)
{
    for (uint8_t i = 0; i < numberOfPixels; i++)
    {
        pixels[i] = chsv;
    }
}