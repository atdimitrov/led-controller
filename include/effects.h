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
        StaticColor(LedPanel &ledPanel, const CHSV color);
        ~StaticColor() override;
        void run() override;
};

class StaticGradient : public Effect
{
    public:
        StaticGradient(LedPanel &ledPanel, const CHSV startColor, const CHSV endColor, const bool reverse);
        ~StaticGradient() override;
        void run() override;
};

class RunningGradient : public Effect
{
    public:
        RunningGradient(LedPanel &ledPanel, const CHSV startColor, const CHSV endColor, const uint8_t speed, const uint8_t spreadFactor, const bool reverse);
        ~RunningGradient() override;
        void run() override;
    private:
        CHSV *gradient;
        const uint8_t speed;
        const bool reverse;
        ValueBouncer valueBouncer;
};

#endif