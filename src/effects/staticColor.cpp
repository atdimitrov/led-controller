#include <FastLED.h>
#include "led-panel.h"
#include "effects.h"

StaticColor::StaticColor(LedPanel &ledPanel, const CHSV color)
    : Effect(ledPanel)
{
    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        ledPanel.sections[i]->setColor(color);
    }
}

StaticColor::~StaticColor()
{
}

void StaticColor::run()
{
}