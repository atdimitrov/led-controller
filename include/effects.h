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
        StaticColor(LedPanel &ledPanel, uint8_t hue);
        ~StaticColor() override;
        void run() override;
    private:
        const uint8_t hue;
};

#endif