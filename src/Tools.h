#pragma once
#include <Arduino.h>

namespace tools
{
    static uint8_t lerp8(uint8_t a, uint8_t b, uint8_t amt)
    {
        uint16_t temp = (uint16_t)b * amt;
        amt = 255-amt;
        temp += (uint16_t)a*amt;
        return temp >> 8;
    }
}