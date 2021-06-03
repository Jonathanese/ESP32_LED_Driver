#pragma once
#include "AnimationController.h"

#define FG AnimationController.Foreground
#define BG AnimationController.Background
#define FOR_PIXELS                                       \
    for (uint8_t strip = 0; strip < GRID_WIDTH; strip++) \
    {                                                    \
        for (uint8_t led = 0; led < GRID_LENGTH; led++)
#define THIS_PIXEL LEDSet.Pixels[strip][led]

using namespace AnimTools;

Animation Off("OFF", 20, [] {
    FOR_PIXELS
    {
        LEDSet.Pixels[strip][led] = Color(0, 0, 0);
    }
    }
});

Animation Solid("none", 40, [] {
    FOR_PIXELS
    {
        LEDSet.Pixels[strip][led] = FG;
    }
    }
});

Animation Fire("fire", 60, []
               {
                   for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
                   {

                       for (uint8_t i = 0; i < GRID_LENGTH; i++)
                       {
                           LEDSet.Pixels[strip][i].param -= FIRE_COOLING;
                           if (LEDSet.Pixels[strip][i].param < 0)
                               LEDSet.Pixels[strip][i].param = 0;
                       }

                       for (uint8_t k = GRID_LENGTH - 1; k >= 2; k--)
                       {
                           LEDSet.Pixels[strip][k].param = ((LEDSet.Pixels[strip][k - 1].param * 5) +
                                                            (LEDSet.Pixels[strip][k - 2].param * 5) +
                                                            (LEDSet.Pixels[stripWraparound(strip + 1)][k - 1].param)) /
                                                           11;
                       }

                       if (random(0, 255) < FIRE_SPARKING)
                       {
                           uint8_t y = random(2);
                           LEDSet.Pixels[strip][y].param += random(64, 80);
                           if (LEDSet.Pixels[strip][y].param > 255)
                               LEDSet.Pixels[strip][y].param = 255;
                       }

                       for (uint8_t j = 0; j < GRID_LENGTH; j++)
                       {
                           LEDSet.Pixels[strip][j] = fireColor(LEDSet.Pixels[strip][j].param, Color(255, 255, 8), FG);
                       }
                   }
               });

Animation Coals("coals", 60, []{
    FOR_PIXELS
    {
        THIS_PIXEL = Color(0,0,0);
        if (roll16(COALS_AMOUNT))
        {
            THIS_PIXEL.param = 255;
        }

        if (THIS_PIXEL.param > 0)
        {
            THIS_PIXEL.Lerp(FG, THIS_PIXEL.param), 
            THIS_PIXEL.param -= 4;
        }
    }}
});

Animation Fireflies("fireflies", 120, [] {
    FOR_PIXELS
    {
        THIS_PIXEL = FG;
        THIS_PIXEL.Lerp(Color(0, 0, 0), 225);

        if (roll16(FIREFLIES_CHANCE))
        {
            THIS_PIXEL.param = 255;
        }

        if (THIS_PIXEL.param > 0)
        {
            THIS_PIXEL.Lerp(Color(255, 255, 0), para(THIS_PIXEL.param));
            THIS_PIXEL.param -= 2;
        }
    }}
});

#define GLOW_IDX LEDSet.Pixels[0][0].param
#define GLOW_DIR LEDSet.Pixels[0][1].param

Animation MovingGlow("moving_glow", 45, [] {
    Color FG2 = FG;
    FG2.Lerp(Color(0, 0, 0), 128);
    FOR_PIXELS
    {
        THIS_PIXEL = FG;

        THIS_PIXEL.Lerp(FG2, clamp(16 * dist(led, GLOW_IDX)));
    }}

    GLOW_IDX = (uint16_t)sin8(GLOW_DIR)*GRID_LENGTH >>8;
    GLOW_DIR++;
    GLOW_DIR %= 255;
});

Animation MovingLight("moving_light", 45, [] {
    Color FG2 = Color(0,0,0);
    FOR_PIXELS
    {
        THIS_PIXEL = FG;

        THIS_PIXEL.Lerp(FG2, clamp(16 * dist(led, GLOW_IDX)));
    }}

    GLOW_IDX = (uint16_t)sin8(GLOW_DIR)*GRID_LENGTH >>8;
    GLOW_DIR++;
    GLOW_DIR %= 255;
});

#define FILMIC_IDX LEDSet.Pixels[0][0].param
Animation Filmic("filmic",100, []{
    FOR_PIXELS
    {
        if (FILMIC_IDX)
            THIS_PIXEL = FG;
        else
            THIS_PIXEL = Color(0, 0, 0);
    }}

    FILMIC_IDX = !(FILMIC_IDX);
});

#define LIGHTNING_IDX LEDSet.Pixels[0][0].param
#define LIGHTNING_BRIGHT LEDSet.Pixels[0][1].param

Animation Lightning("lightning", 30, []{
    FOR_PIXELS
    {
        THIS_PIXEL = FG;
        THIS_PIXEL.Lerp(Color(255, 255, 255), LIGHTNING_BRIGHT);
    }}
    if(LIGHTNING_IDX)
    {
    LIGHTNING_BRIGHT = random8(LIGHTNING_BRIGHT);
    LIGHTNING_IDX -= 4;
    }
    else
    {
    if (roll16(1024))
    {
        LIGHTNING_IDX = 255;
    }
    }
});

Animation Aurora("aurora", 60, []{
    FOR_PIXELS
    {

        THIS_PIXEL.param += random8(10);
        THIS_PIXEL.param -= random8(5);
        THIS_PIXEL.param = clamp(THIS_PIXEL.param);

        THIS_PIXEL = FG;
        THIS_PIXEL.Lerp(Color(0, 255, 128), THIS_PIXEL.param);
    }}
});