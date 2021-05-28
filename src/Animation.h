#pragma once
#include <Arduino.h>
#include "Render.h"

#define ANIMATION_MAX 20

class Animation
{
public:
    Animation(String name, uint8_t framerate, void (*new_animator)());
    Animation();
    String getName();
    uint8_t getFrameRate();
    static Animation getAnimation(uint8_t idx);
    static Animation getAnimation(String name);
    void (*animator)() = _nop;

private:
    String _name;
    uint8_t _framerate;
    void _add_anim_to_list();
    static void _nop(){};
};

extern Animation *AnimationList[];

#define MIDPOINT 96
namespace AnimTools
{
    const uint8_t b_m16_interleave[] = { 0, 49, 49, 41, 90, 27, 117, 10 };


    static Color fireColor(uint8_t heat, Color Spark, Color Flame)
    {
        if (heat <= MIDPOINT)
        {
            Flame.Lerp(Color(0, 0, 0), (((MIDPOINT - heat) * 255 / MIDPOINT)));
            return Flame;
        }
        else if (heat < 255)
        {
            Flame.Lerp(Spark, (((heat - MIDPOINT) * 255 / (255 - MIDPOINT))));
            return Flame;
        }
        else
        {
            return Spark;
        }
    }

    static uint8_t stripWraparound(int idx)
    {
        while (idx >= GRID_WIDTH)
            idx -= GRID_WIDTH;
        while (idx < 0)
            idx += GRID_WIDTH;
    }

    // Random functions from FastLED
    static uint16_t random16()
    {
        static uint16_t seed = 1432;
        seed = (seed << 11) + (seed << 2) + seed + (uint16_t)13849;
        return seed;
    }

    static uint16_t random16(uint16_t lim)
    {
        uint16_t r = random16();
        uint32_t p = (uint32_t)lim * (uint32_t)r;
        r = p >> 16;
        return r;
    }

    static uint16_t random16(uint16_t min, uint16_t lim)
    {
        uint16_t delta = lim - min;
        uint16_t r = random16(delta) + min;
        return r;
    }

    static uint8_t random8()
    {
        uint16_t r = random16();
        return (uint8_t)(((uint8_t)(r & 0xFF)) +
                         ((uint8_t)(r >> 8)));
    }

    static uint8_t random8(uint8_t lim)
    {
        uint8_t r = random8();
        r = (r * lim) >> 8;
        return r;
    }

    static uint8_t random8(uint8_t min, uint8_t lim)
    {
        uint8_t delta = lim - min;
        uint8_t r = random8(delta) + min;
        return r;
    }

    static bool roll(uint8_t value)
    {
        return (random8() <= value);
    }

    static bool roll16(uint16_t value)
    {
        return (random16() <= value);
    }

    static uint8_t para(uint8_t x)
    {
        uint16_t X = x;
        uint16_t output;

        output = 4 * X;
        output -= X * X >> 6;
        if (output > 255)
            output = 255;
        return output;
    }

    static uint8_t dist(uint8_t x, uint8_t y)
    {
        if (x > y)
        {
            return x - y;
        }
        return y - x;
    }

    static uint8_t clamp(int x)
    {
        if (x > 255)
            return 255;
        if (x < 0)
            return 0;
        return x;
    }

    static uint8_t sin8(uint8_t theta)
    {
        uint8_t offset = theta;
        if (theta & 0x40)
        {
            offset = (uint8_t)255 - offset;
        }
        offset &= 0x3F; // 0..63

        uint8_t secoffset = offset & 0x0F; // 0..15
        if (theta & 0x40)
            ++secoffset;

        uint8_t section = offset >> 4; // 0..3
        uint8_t s2 = section * 2;
        const uint8_t *p = b_m16_interleave;
        p += s2;
        uint8_t b = *p;
        ++p;
        uint8_t m16 = *p;

        uint8_t mx = (m16 * secoffset) >> 4;

        int8_t y = mx + b;
        if (theta & 0x80)
            y = -y;

        y += 128;

        return y;
    }
};