#ifndef EFFECTS_H
#define EFFECTS_H

#include "led-panel.h"

class Effect
{
    public:
        Effect(LedPanel &ledPanel);
        virtual ~Effect();
        virtual void run() = 0;
    protected:
        LedPanel &ledPanel;
};

class StaticColor : public Effect
{
    public:
        StaticColor(LedPanel &ledPanel, const CRGB color);
        ~StaticColor() override;
        void run() override;
    private:
        const CRGB color;
};

class Gradient : public Effect
{
    public:
        Gradient(LedPanel &ledPanel, const CRGB startColor, const CRGB endColor);
        ~Gradient() override;
        void run() override;
    private:
        const CRGBPalette16 palette;
        const int step;
};

#endif