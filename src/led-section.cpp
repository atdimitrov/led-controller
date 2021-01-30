#include <FastLED.h>
#include "led-section.h"

LedSection::LedSection(uint8_t numberOfPixels, CRGB *pixels)
    : numberOfPixels(numberOfPixels),
      pixels(pixels)
{
}

void LedSection::setColor(CRGB color)
{
    for (uint8_t i = 0; i < numberOfPixels; i++)
    {
        pixels[i] = color;
    }
}