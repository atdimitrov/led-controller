#include <Arduino.h>
#include <FastLED.h>
#include "effects.h"

PaletteRandomness::PaletteRandomness(LedPanel &ledPanel, const CHSV color1, const CHSV color2, const CHSV color3, const CHSV color4, const uint8_t speed)
    : Effect(ledPanel),
      palette(CHSVPalette16(color1, color2, color3, color4)),
      speed(speed),
      paletteIndices(new uint8_t[ledPanel.numberOfSections])
{
    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        paletteIndices[i] = random8();
    }
}

PaletteRandomness::~PaletteRandomness()
{
    delete[] paletteIndices;
}

void PaletteRandomness::run()
{
    EVERY_N_MILLIS_I(paletteRandomnessTimer, 1)
    {
        paletteRandomnessTimer.setPeriod(20 * (10 - (speed - 1)));

        for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
        {
            paletteIndices[i]++;

            ledPanel.sections[i]->setColor(ColorFromPalette(palette, paletteIndices[i]));
        }
    }
}