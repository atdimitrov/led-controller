#include "effects.h"

Gradient::Gradient(LedPanel &ledPanel, const CRGB startColor, const CRGB endColor)
    : Effect(ledPanel),
      palette(CRGBPalette16(startColor, endColor)),
      step(255 / ledPanel.numberOfSections)
{
}

Gradient::~Gradient()
{
}

void Gradient::run()
{
    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        CRGB color = ColorFromPalette(palette, i * step);
        ledPanel.sections[i]->setColor(color);
    }
}