#pragma once
#define CEILING_STRIP


//Generally constant across all devices
#define COLORS_PER_LED 4
#define LED_STRIP_PINS \
    {                  \
        12, 14, 26, 25 \
    }




//Config for specific layouts

//Living room vertical pole
#ifdef POLE1

#define SECRET_MQTT_STATE_TOPIC "ledstrip/pole1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/pole1/set"
#define SECRET_MQTT_NAME "pole1"

#define LED_PER_STRIP 60
#define LED_STRIPS 4

#define GRID_WIDTH 4
#define GRID_LENGTH 60
#define STRIP_LAYOUT                                 \
    {                                                \
        {0, 59}, {60, 119}, {120, 179}, { 180, 239 } \
    }

#define TEMPORAL_DITHER_COUNT 10
#define SPATIAL_DITHER_COUNT 7

#define FIRE_COOLING 5
#define FIRE_SPARKING 40

#define COALS_AMOUNT 1024

#define FIREFLIES_CHANCE 1

#endif

//Living room vertical pole
#ifdef CEILING_STRIP

#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip2"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip2/set"
#define SECRET_MQTT_NAME "strip2"

#define LED_PER_STRIP 60
#define LED_STRIPS 4

#define GRID_WIDTH 2
#define GRID_LENGTH 113
#define STRIP_LAYOUT                                 \
    {                                                \
        {112,0},{113,225} \
    }

#define TEMPORAL_DITHER_COUNT 10
#define SPATIAL_DITHER_COUNT 7

#define FIRE_COOLING 3
#define FIRE_SPARKING 50

#define COALS_AMOUNT 768

#define FIREFLIES_CHANCE 1

#endif
