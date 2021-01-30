#include "effects.h"

StaticColor::StaticColor(LedPanel &ledPanel, const uint8_t hue)
    : Effect(ledPanel),
      hue(hue)
{
}

StaticColor::~StaticColor()
{
}

void StaticColor::run()
{
    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        ledPanel.sections[i]->setColor(hue);
    }
}
