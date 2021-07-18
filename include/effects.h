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
        StaticGradient(LedPanel &ledPanel, const CHSV startColor, const CHSV endColor, const bool reverse, const TGradientDirectionCode direction);
        ~StaticGradient() override;
        void run() override;
};

class RunningGradient : public Effect
{
    public:
        RunningGradient(LedPanel &ledPanel, const CHSV startColor, const CHSV endColor, const uint8_t speed, const uint8_t spreadFactor, const bool reverse, const TGradientDirectionCode direction);
        ~RunningGradient() override;
        void run() override;
    private:
        CHSV *gradient;
        const uint8_t speed;
        const bool reverse;
        ValueBouncer valueBouncer;
};

class Starlight : public Effect
{
    public:
        Starlight(LedPanel &ledPanel, const CHSV primaryColor, const CHSV secondaryColor, const uint8_t speed);
        ~Starlight() override;
        void run() override;
    private:
        CHSV *gradient;
        const uint8_t speed;
        class StarlightSection
        {
            public:
                StarlightSection(LedSection *section, CHSV *gradient);
                bool IsActive();
                void Activate();
                void Tick();
            private:
                LedSection *section;
                CHSV *gradient;
                bool isActive;
                ValueBouncer valueBouncer;
                uint8_t cyclesPerChange;
                uint8_t currentCycles;
        };
        StarlightSection** sections;
};

class PaletteRandomness : public Effect
{
    public:
        PaletteRandomness(LedPanel &ledPanel, const CHSV color1, const CHSV color2, const CHSV color3, const CHSV color4, const uint8_t speed);
        ~PaletteRandomness() override;
        void run() override;
    private:
        const CHSVPalette16 palette;
        const uint8_t speed;
        uint8_t *paletteIndices;
};

#endif