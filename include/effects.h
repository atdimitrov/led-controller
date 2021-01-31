#ifndef EFFECTS_H
#define EFFECTS_H

#include "led-panel.h"
#include "valueBouncer.h"

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

class StaticGradient : public Effect
{
    public:
        StaticGradient(LedPanel &ledPanel, const CRGB startColor, const CRGB endColor);
        ~StaticGradient() override;
        void run() override;
};

class RunningGradient : public Effect
{
    public:
        RunningGradient(LedPanel &ledPanel, const CRGB startColor, const CRGB endColor, const uint8_t speed, const uint8_t spreadFactor);
        ~RunningGradient() override;
        void run() override;
    private:
        CRGB *gradient;
        const uint8_t speed;
        ValueBouncer valueBouncer;
};

#endif