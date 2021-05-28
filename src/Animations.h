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

Animation Solid("none", 20, [] {
    FOR_PIXELS
    {
        LEDSet.Pixels[strip][led] = FG;
    }
    }
});

#define FIRE_COOLING 5
#define FIRE_SPARKING 40

Animation Fire("fire", 60, [] {
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

#define COALS_SPARKING 20
#define COALS_COOLING 10

Animation Coals("coals", 60, [] {
    for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
    {

        for (uint8_t i = 0; i < GRID_LENGTH; i++)
        {
            if (random(255) < LEDSet.Pixels[strip][i].param)
            {
                LEDSet.Pixels[strip][i].param -= 1;
                if (LEDSet.Pixels[strip][i].param < 0)
                    LEDSet.Pixels[strip][i].param = 0;
            }
        }

        for (uint8_t k = GRID_LENGTH - 1; k >= 2; k--)
        {
            LEDSet.Pixels[strip][k].param = ((LEDSet.Pixels[strip][k - 2].param) +
                                             (LEDSet.Pixels[strip][k - 1].param) +
                                             (LEDSet.Pixels[strip][k + 1].param) +
                                             (LEDSet.Pixels[strip][k + 2].param) +
                                             (LEDSet.Pixels[stripWraparound(strip + 1)][k].param) +
                                             (LEDSet.Pixels[stripWraparound(strip + 1)][k].param)) /
                                            6;
        }

        if (random(0, 255) < COALS_SPARKING)
        {
            uint8_t y = random(GRID_LENGTH);
            LEDSet.Pixels[strip][y].param += random(0, 32);
            if (LEDSet.Pixels[strip][y].param > 255)
                LEDSet.Pixels[strip][y].param = 255;
        }

        for (uint8_t j = 0; j < GRID_LENGTH; j++)
        {
            LEDSet.Pixels[strip][j] = fireColor(LEDSet.Pixels[strip][j].param, Color(255, 255, 8), FG);
        }
    }
});

#define FIREFLIES_CHANCE 1
Animation Fireflies("fireflies", 120, [] {
    FOR_PIXELS
    {
        THIS_PIXEL = FG;
        THIS_PIXEL.Lerp(Color(0, 0, 0), 192);

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
    FG2.Lerp(Color(0,0,0), 128);
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
Animation Filmic("filmic", 90, []{
    FOR_PIXELS{
    if(FILMIC_IDX)
        THIS_PIXEL = FG;
    else
        THIS_PIXEL = Color(0,0,0);
    }}

    FILMIC_IDX = !(FILMIC_IDX);

});