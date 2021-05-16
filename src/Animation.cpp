#include "Animation.h"

_animation_controller AnimationController;

String printcolor(Color c)
{
    return String(c.r) + " " + String(c.g) + " " + String(c.b);
}

_animation_controller::_animation_controller()
{
}

_animation_controller::~_animation_controller()
{
}

void _animation_controller::loop()
{
    if (_frameTimer.repeat(_frame_time))
    {
        _frame_loop();
    }
    LEDSet.Show();
}

void _animation_controller::_frame_loop()
{
    _frame_action();
    //Before we show, check if we are transitioning, and perform the transition.
    _transition_frame();
}

void _animation_controller::_transition_frame()
{
    if (_transition_frames_remaining > 0)
    {
        uint8_t lerp_amt = (uint16_t)_transition_frames_remaining * 255 / _transition_frames_total;
        uint8_t hits = 0;
        for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
        {
            for (uint8_t led = 0; led < GRID_LENGTH; led++)
            {
                LEDSet.Pixels[strip][led].Lerp(_back_pixels[strip][led], lerp_amt);
                hits++;
            }
        }

        _transition_frames_remaining--;
    }
}

void _animation_controller::TransitionTo(void (*anim_function)(), uint8_t frames)
{
    //Copy current frame to _back_pixels so we have colors to transition from.
    memcpy(_back_pixels, LEDSet.Pixels, sizeof(Pixel) * GRID_WIDTH * GRID_LENGTH);
    _transition_frames_remaining = frames;
    _transition_frames_total = frames;

    //Switch over to the new animation
    _frame_action = anim_function;
}

void _animation_controller::setFrameRate(uint8_t fps)
{
    _frame_time = 1000 / fps;
}