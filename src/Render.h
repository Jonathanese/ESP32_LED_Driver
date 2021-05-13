#pragma once
//#define FULL_DMA_BUFFER
#include "CONFIG.h"
#include <Arduino.h>
#include <I2SClocklessLedDriver.h>
#include "DebugMessage.h"
#include "Color.h"
#include "Color_Correction.h"

struct _stripmap
{
    uint8_t start;
    uint8_t stop;
};

extern _stripmap _striplayout[GRID_WIDTH];

class LEDSet
{
    //Members
public:
    Pixel Pixels[GRID_WIDTH][GRID_LENGTH];

private:
    I2SClocklessLedDriver _driver;
    uint8_t _stripadr[GRID_WIDTH][GRID_LENGTH];
    uint8_t _outputvalues[LED_PER_STRIP * LED_STRIPS * COLORS_PER_LED];
    #ifdef BRIGHTNESS_GAMMA
    uint16_t _brightness_gamma = 16384;
        uint32_t _den = CORRECTION_PRECISION * CORRECTION_PRECISION * 255; //Pull out correction precision for each LUT for brightness and color. ~24b
    #else
    uint8_t _brightness = 255;
        uint32_t _den = CORRECTION_PRECISION * 255; //Pull out correction precision for each LUT for brightness and color. ~24b
    #endif

    #if (TEMPORAL_DITHER_COUNT > 1) && (SPATIAL_DITHER_COUNT > 1)
    uint32_t _num = TEMPORAL_DITHER_COUNT*SPATIAL_DITHER_COUNT; 
    #elif (TEMPORAL_DITHER_COUNT > 1)
    uint32_t _num = TEMPORAL_DITHER_COUNT;
    #elif (SPATIAL_DITHER_COUNT > 1)
    uint32_t _num = SPATIAL_DITHER_COUNT;
    #endif


    //Methods
public:
    LEDSet();
    ~LEDSet();
    void Init();
    void Show();
    void setBrightness(uint8_t new_bright);
    uint8_t Count();
    uint8_t GlobalCount();

private:
    void _allocate_grid();
};

extern LEDSet LEDS;