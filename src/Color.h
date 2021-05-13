#pragma once
#include <Arduino.h>

class Color
{
    public:
    Color();
    Color(uint8_t _r, uint8_t _g, uint8_t _b);
    ~Color();

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Pixel: public Color{
    public:
    uint8_t p;
    void operator=(Color _color);
};