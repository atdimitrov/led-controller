#include <FastLED.h>
#include "led-panel.h"
#include "effects.h"

StaticGradient::StaticGradient(LedPanel &ledPanel, const CHSV startColor, const CHSV endColor, const bool reverse, const TGradientDirectionCode direction)
    : Effect(ledPanel)
{
    CHSV gradient[ledPanel.numberOfSections];
    fill_gradient(gradient, ledPanel.numberOfSections, startColor, endColor, direction);

    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        uint8_t sectionIndex = i;
        if (reverse) sectionIndex = ledPanel.numberOfSections - i - 1;

        ledPanel.sections[sectionIndex]->setColor(gradient[i]);
    }
}

StaticGradient::~StaticGradient()
{
}

void StaticGradient::run()
{
}