#include <Arduino.h>
#include <FastLED.h>
#include "led-panel.h"
#include "led-section.h"

void AddLeds(uint8_t pin, CRGB *leds, uint8_t numberOfLeds)
{
    switch (pin)
    {
        case 17:
            FastLED.addLeds<NEOPIXEL, 17>(leds, numberOfLeds);
            break;
        default:
            throw "AddLeds() does not support the provided pin.";
    }
}

LedPanel::LedPanel(uint8_t pin, uint8_t numberOfSections, uint numberOfPixelsPerSection)
    : numberOfSections(numberOfSections),
      numberOfPixelsPerSection(numberOfPixelsPerSection),
      numberOfPixels(numberOfSections * numberOfPixelsPerSection),
      pixels(new CRGB[numberOfPixels])
{
    AddLeds(pin, pixels, numberOfPixels);

    sections = (LedSection **)malloc(sizeof(LedSection *) * numberOfSections);
    if (!sections)
    {
        throw "Memory allocation failed.";
    }

    for (uint8_t i = 0; i < numberOfSections; i++)
    {
        sections[i] = new LedSection(numberOfPixelsPerSection, pixels + (i * numberOfPixelsPerSection));
    }
}

LedPanel::~LedPanel()
{
    delete[] pixels;

    for (uint8_t i = 0; i < numberOfSections; i++)
    {
        delete sections[i];
    }

    free(sections);
}