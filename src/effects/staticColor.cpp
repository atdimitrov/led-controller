#include "effects.h"

StaticColor::StaticColor(LedPanel &ledPanel, const CRGB color)
    : Effect(ledPanel),
      color(color)
{
}

StaticColor::~StaticColor()
{
}

void StaticColor::run()
{
    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        ledPanel.sections[i]->setColor(color);
    }
}
