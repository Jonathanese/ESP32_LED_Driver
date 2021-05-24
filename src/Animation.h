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

};