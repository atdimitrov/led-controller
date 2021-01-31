#include <FastLED.h>
#include "led-panel.h"
#include "effects.h"

StaticGradient::StaticGradient(LedPanel &ledPanel, const CRGB startColor, const CRGB endColor)
    : Effect(ledPanel)
{
    CRGB gradient[ledPanel.numberOfSections];
    fill_gradient_RGB(gradient, ledPanel.numberOfSections, startColor, endColor);

    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        ledPanel.sections[i]->setColor(gradient[i]);
    }
}

StaticGradient::~StaticGradient()
{
}

void StaticGradient::run()
{
}