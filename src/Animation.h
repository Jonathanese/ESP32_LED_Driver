#pragma once
#include "Render.h"
#include "StopWatch.h"

//Encapsulation in a class may not be totally necessary
class _animation_controller
{   //Members
    public:

    private:
    uint8_t _transition_frames_total;
    uint8_t _transition_frames_remaining;
    //Use Pixels here so we can use memcpy
    Pixel _back_pixels[GRID_WIDTH][GRID_LENGTH];
    StopWatch _frameTimer;
    uint8_t _frame_time;


    //Methods
    public:
    _animation_controller();
    ~_animation_controller();
    void loop();
    void setFrameRate(uint8_t fps);
    void TransitionTo(void(*anim_function)(), uint8_t frames);

    private:
    void (*_frame_action)(void);
    void _transition_frame();
    void _frame_loop();

};

extern _animation_controller AnimationController;
