#include <FastLED.h>
#include "led-panel.h"
#include "effects.h"

RunningGradient::RunningGradient(LedPanel &ledPanel, const CHSV startColor, const CHSV endColor, const uint8_t speed, const uint8_t spreadFactor, const bool reverse, const TGradientDirectionCode direction)
    : Effect(ledPanel),
      gradient(new CHSV[ledPanel.numberOfSections * spreadFactor]),
      speed(speed),
      reverse(reverse),
      valueBouncer(ValueBouncer(0, (ledPanel.numberOfSections * spreadFactor) - 1))
{
    fill_gradient(gradient, ledPanel.numberOfSections * spreadFactor, startColor, endColor, direction);
}

RunningGradient::~RunningGradient()
{
    delete[] gradient;
}

void RunningGradient::run()
{
    EVERY_N_MILLIS_I(runningGradientTimer, 1)
    {
        runningGradientTimer.setPeriod(20 * (10 - (speed - 1)));

        ValueBouncer currentValueBouncer = valueBouncer;
        for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
        {
            uint8_t sectionIndex = i;
            if (reverse) sectionIndex = ledPanel.numberOfSections - i - 1;

            CHSV color = gradient[currentValueBouncer.next()];
            ledPanel.sections[sectionIndex]->setColor(color);
        }

        valueBouncer.next();
    }
}