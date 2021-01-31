#ifndef VALUE_BOUNCER_H
#define VALUE_BOUNCER_H

#include <Arduino.h>

struct ValueBouncer
{
    public:
        ValueBouncer(const uint8_t from, const uint8_t to);
        ValueBouncer(const ValueBouncer &valueBouncer);
        uint8_t next();
    private:
        const uint8_t from;
        const uint8_t to;
        uint8_t step;
        uint8_t current;
};

#endif