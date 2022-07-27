#pragma once
#include "Render.h"
#include "StopWatch.h"
#include "Animation.h"
#include <ArduinoJson.h>
#include "CONFIG.h"
#include "MQTT.h"

static void MQTT_Update_callback(char *topic, uint8_t *payload, unsigned int length);

//Encapsulation in a class may not be totally necessary
class _animation_controller
{ //Members
public:
    Color Foreground;

private:
    uint16_t _transition_frames_total;
    uint16_t _transition_frames_remaining;
    uint8_t _old_brightness;
    uint8_t _brightness;
    //Use Pixels here so we can use memcpy
    Pixel _back_pixels[GRID_WIDTH][GRID_LENGTH];
    StopWatch _frameTimer;
    uint8_t _frame_time;

    //Methods
public:
    void init();
    void loop();
    void setFrameRate(uint8_t fps);
    void TransitionTo(Animation new_animation, uint32_t transition_time, uint8_t brightness);
    void TransitionTo(String animation_name, uint32_t transition_time, uint8_t brightness, Color foreground);
    void ParseJson(char *topic, byte *payload, unsigned int length);
    
private:
    StaticJsonDocument<512> _json_doc;
    Animation _current_animation;
    void _send_state();
    void _transition_frame();
    void _frame_loop();
};

extern _animation_controller AnimationController;
