#pragma once
#define POLE1


//Generally constant across all devices
#define COLORS_PER_LED 4
#define LED_STRIP_PINS \
    {                  \
        12, 14, 26, 25 \
    }




//Config for specific layouts

//Living room vertical pole
#ifdef POLE1

#define LED_PER_STRIP 60
#define LED_STRIPS 4

#define GRID_WIDTH 4
#define GRID_LENGTH 60
#define STRIP_LAYOUT                                 \
    {                                                \
        {0, 59}, {60, 119}, {120, 179}, { 180, 239 } \
    }

#define TEMPORAL_DITHER_COUNT 8
#define SPATIAL_DITHER_COUNT 2

#endif

//Strip across bedroom ceiling
#ifdef STRIP_BEDROOM

#define LED_PER_STRIP 60
#define LED_STRIPS 4

#define GRID_WIDTH 2
#define GRID_LENGTH 120
#define STRIP_LAYOUT                                 \
    {                                                \
        {119,0}, {120,239} \
    }

#define DITHER_COUNT 8

#endif
