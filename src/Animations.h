#pragma once
#include "AnimationController.h"

#define FG AnimationController.Foreground
#define BG AnimationController.Background

Animation Off("OFF", 20, [] {
    for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
    {
        for (uint8_t led = 0; led < GRID_LENGTH; led++)
        {
            LEDSet.Pixels[strip][led] = Color(0, 0, 0);
        }
    }
});

Animation Solid("none", 20, [] {
    for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
    {
        for (uint8_t led = 0; led < GRID_LENGTH; led++)
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
                                             (LEDSet.Pixels[AnimTools::stripWraparound(strip + 1)][k - 1].param)) /
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
            LEDSet.Pixels[strip][j] = AnimTools::fireColor(LEDSet.Pixels[strip][j].param, Color(255, 255, 8), FG);
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
            if (random(255) < COALS_COOLING)
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
                                             (LEDSet.Pixels[AnimTools::stripWraparound(strip + 1)][k].param) +
                                             (LEDSet.Pixels[AnimTools::stripWraparound(strip + 1)][k].param)) /
                                            6;
        }

        if (random(0, 255) < COALS_SPARKING)
        {
            uint8_t y = random(GRID_LENGTH);
            LEDSet.Pixels[strip][y].param += random(32, 64);
            if (LEDSet.Pixels[strip][y].param > 255)
                LEDSet.Pixels[strip][y].param = 255;
        }

        for (uint8_t j = 0; j < GRID_LENGTH; j++)
        {
            LEDSet.Pixels[strip][j] = AnimTools::fireColor(LEDSet.Pixels[strip][j].param, Color(255, 255, 8), FG);
        }
    }
});
