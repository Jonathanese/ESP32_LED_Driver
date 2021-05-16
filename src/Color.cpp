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

void Color::Lerp(Color C, uint8_t amount)
{
    r = tools::lerp8(r, C.r, amount);
    g = tools::lerp8(g, C.g, amount);
    b = tools::lerp8(b, C.b, amount);
}

void Pixel::operator=(Color _color)
{
    rgb = _color.rgb;
}