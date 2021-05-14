#pragma once
#include <Arduino.h>

class Color
{
public:
    Color();
    Color(uint8_t _r, uint8_t _g, uint8_t _b);
    ~Color();
    union
    {
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };
        uint32_t rgb;
    };
};

class Pixel : public Color
{
public:
    int16_t param;
    void operator=(Color _color);
};