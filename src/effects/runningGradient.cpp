#include <FastLED.h>
#include "led-panel.h"
#include "effects.h"

RunningGradient::RunningGradient(LedPanel &ledPanel, const CRGB startColor, const CRGB endColor, const uint8_t speed, const uint8_t spreadFactor)
    : Effect(ledPanel),
      gradient(new CRGB[ledPanel.numberOfSections * spreadFactor]),
      speed(speed),
      valueBouncer(ValueBouncer(0, (ledPanel.numberOfSections * spreadFactor) - 1))
{
    fill_gradient_RGB(gradient, ledPanel.numberOfSections * spreadFactor, startColor, endColor);
}

RunningGradient::~RunningGradient()
{
    delete[] gradient;
}

void RunningGradient::run()
{
    EVERY_N_MILLIS_I(runningGradientTimer, 1)
    {
        runningGradientTimer.setPeriod(20 * (10 - speed - 1));

        ValueBouncer currentValueBouncer = valueBouncer;
        for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
        {
            CRGB color = gradient[currentValueBouncer.next()];
            ledPanel.sections[i]->setColor(color);
        }

        valueBouncer.next();
    }
}