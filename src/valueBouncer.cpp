#include <Arduino.h>
#include "valueBouncer.h"

ValueBouncer::ValueBouncer(const uint8_t from, const uint8_t to)
    : from(from),
      to(to),
      step(1),
      current(from)
{
}

ValueBouncer::ValueBouncer(const ValueBouncer &valueBouncer)
    : from(valueBouncer.from),
      to(valueBouncer.to),
      step(valueBouncer.step),
      current(valueBouncer.current)
{
}

uint8_t ValueBouncer::next()
{
    uint8_t result = current;

    current += step;
    if (current == from || current == to)
    {
        step = -step;
    }

    return result;
}