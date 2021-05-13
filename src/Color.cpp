#include "Color.h"

Color::Color()
{

}

Color::Color(uint8_t _r, uint8_t _g, uint8_t _b)
{
    r = _r;
    g = _g;
    b = _b;
}

Color::~Color(){}

void Pixel::operator=(Color _color)
{
    r = _color.r;
    g = _color.g;
    b = _color.b;
}

