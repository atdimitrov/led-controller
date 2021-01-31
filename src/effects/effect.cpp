#include "led-panel.h"
#include "effects.h"

Effect::Effect(LedPanel &ledPanel)
    : ledPanel(ledPanel)
{
}

Effect::~Effect()
{
}