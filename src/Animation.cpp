#include "Animation.h"

Animation *AnimationList[ANIMATION_MAX];

Animation::Animation(String name, uint8_t framerate, void (*new_animator)())
{
    _name = name;
    _framerate = framerate;
    animator = new_animator;
    _add_anim_to_list();
}

Animation::Animation()
{
    _name = "";
    _framerate = 25;
    animator = [] {
        for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
        {
            for (uint8_t led = 0; led < GRID_LENGTH; led++)
            {
                LEDSet.Pixels[strip][led].Lerp(Color(0,0,0), 32);
                if(led == LEDSet.Pixels[0][0].param)
                {
                    LEDSet.Pixels[strip][led] = Color(255,0,0);
                }
            }
        }
        LEDSet.Pixels[0][0].param++;
        LEDSet.Pixels[0][0].param %= GRID_LENGTH;
    };
    _add_anim_to_list();
}

String Animation::getName()
{
    return _name;
}

uint8_t Animation::getFrameRate()
{
    return _framerate;
}

void Animation::_add_anim_to_list()
{

    bool animexists = false;
    uint8_t anim_index = 0;
    for (; anim_index < ANIMATION_MAX; anim_index++)
    {
        if (AnimationList[anim_index] == NULL)
        {
            break;
        }
        if (this->getName() == AnimationList[anim_index]->getName())
        {
            //Animation already exists
            animexists = true;
            break;
        }
    }

    if (!animexists)
    {
        AnimationList[anim_index] = this;
    }
}

Animation Animation::getAnimation(uint8_t idx)
{
    return *AnimationList[idx];
}

Animation Animation::getAnimation(String name)
{
    for (uint8_t i = 0; i < ANIMATION_MAX; i++)
    {
        if (AnimationList[i] != NULL)
        {
            if (AnimationList[i]->getName() == name)
            {
                return *AnimationList[i];
            }
        }
    }

    Debug.Message(DM_ERROR, "Animation Not Found: " + name);
    return getAnimation("NF");
}