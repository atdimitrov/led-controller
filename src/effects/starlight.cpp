#include <Arduino.h>
#include <FastLED.h>
#include "effects.h"

#define GRADIENT_SMOOTHNESS 50

Starlight::Starlight(LedPanel &ledPanel, const CHSV primaryColor, const CHSV secondaryColor, const uint8_t speed)
    : Effect(ledPanel),
      gradient(new CHSV[GRADIENT_SMOOTHNESS]),
      speed(speed)
{
    fill_gradient(gradient, GRADIENT_SMOOTHNESS, primaryColor, secondaryColor, SHORTEST_HUES);

    sections = (StarlightSection **)malloc(sizeof(StarlightSection *) * ledPanel.numberOfSections);
    if (!sections)
    {
        throw "Memory allocation failed.";
    }

    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        sections[i] = new StarlightSection(ledPanel.sections[i], gradient);
    }
}

Starlight::~Starlight()
{
    delete[] gradient;

    for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
    {
        delete sections[i];
    }

    free(sections);
}

void Starlight::run()
{
    const uint8_t tickTimerPeriod = 10 * (10 - (speed - 1));

    EVERY_N_MILLIS_I(tickTimer, 1)
    {
        tickTimer.setPeriod(tickTimerPeriod);
        
        for (uint8_t i = 0; i < ledPanel.numberOfSections; i++)
        {
            sections[i]->Tick();
        }
    }

    EVERY_N_MILLIS_I(spawnTimer, 1)
    {
        spawnTimer.setPeriod(tickTimerPeriod * 2);

        uint8_t random = random8(ledPanel.numberOfSections);
        if (!sections[random]->IsActive())
        {
            sections[random]->Activate();
        }
    }
}

Starlight::StarlightSection::StarlightSection(LedSection *section, CHSV *gradient)
    : section(section),
      gradient(gradient),
      isActive(false),
      valueBouncer(ValueBouncer(0, GRADIENT_SMOOTHNESS - 1))
{
    section->setColor(gradient[valueBouncer.next()]);
}

bool Starlight::StarlightSection::IsActive()
{
    return isActive;
}

void Starlight::StarlightSection::Activate()
{
    // [1, 6)
    cyclesPerChange = random8(1, 6);
    currentCycles = 0;
    isActive = true;
}

void Starlight::StarlightSection::Tick()
{
    if (isActive)
    {
        currentCycles++;
        if (currentCycles == cyclesPerChange)
        {
            currentCycles = 0;

            uint8_t nextGradientIndex = valueBouncer.next();
            section->setColor(gradient[nextGradientIndex]);

            if (nextGradientIndex == 0)
            {
                isActive = false;
            }
        }
    }
}