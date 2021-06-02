#include "Render.h"

_led_set LEDSet;
_stripmap _striplayout[GRID_WIDTH] = STRIP_LAYOUT;

_led_set::_led_set()
{
    _allocate_grid();
}

_led_set::~_led_set()
{
}

void _led_set::Init()
{
    int _pinlist[] = LED_STRIP_PINS;
    _driver.initled(_outputvalues, _pinlist, LED_STRIPS, LED_PER_STRIP, ORDER_GRBW);
    _driver.setBrightness(255);
}

void _led_set::setBrightness(uint8_t new_bright)
{
    _brightness = new_bright;
}

uint8_t _led_set::getBrightness()
{
    return _brightness;
}

void _led_set::Show()
{
    static uint32_t d_c; //Counter for dithering. Used to alter how things are rounded when we divide.
    static uint32_t sd_c;
    uint32_t p_r; //Temporary values for processing channels
    uint32_t p_g;
    uint32_t p_b;
    uint32_t p_w;

#if SPATIAL_DITHER_COUNT > 1
    sd_c++;
    if (sd_c >= SPATIAL_DITHER_COUNT)
    {
        sd_c -= SPATIAL_DITHER_COUNT;
    }

#endif
    for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
    {
        for (uint8_t led = 0; led < GRID_LENGTH; led++)
        {
            //16b
            p_r = (uint32_t)GAMMA_LUT[Pixels[strip][led].r];
            p_g = (uint32_t)GAMMA_LUT[Pixels[strip][led].g];
            p_b = (uint32_t)GAMMA_LUT[Pixels[strip][led].b];

//Mulgiply brightness. Multiplication should happen before division to minimize precision loss.

            //24b
            p_r = p_r * _brightness;
            p_g = p_g * _brightness;
            p_b = p_b * _brightness;

            p_w = min(p_r, min(p_g, p_b));
            p_r -= p_w;
            p_g -= p_w;
            p_b -= p_w;

            //Divide first to avoid overrun

            //Scale values according to color correction / white balance
            p_r = p_r * CORRECTION_R >> 8;
            p_g = p_g * CORRECTION_G >> 8;
            p_b = p_b * CORRECTION_B >> 8;
            p_w = p_w * CORRECTION_W >> 8;

            //Multiply/Divide Constants
            p_r = p_r * _num / _den;
            p_g = p_g * _num / _den;
            p_b = p_b * _num / _den;
            p_w = p_w * _num / _den;

            /*
            * DITHERING SECTION
            * In this case, the loss of precision is something we specifically want.
            *
            * */

#if SPATIAL_DITHER_COUNT > 1
            //Increment value based on position of pixel
            p_r = p_r + sd_c;
            p_g = p_g + sd_c;
            p_b = p_b + sd_c;
            p_w = p_w + sd_c;

            //Divide to enforce quantization in space
            p_r /= SPATIAL_DITHER_COUNT;
            p_g /= SPATIAL_DITHER_COUNT;
            p_b /= SPATIAL_DITHER_COUNT;
            p_w /= SPATIAL_DITHER_COUNT;
#endif

#if TEMPORAL_DITHER_COUNT > 1
            //Increment value based on time
            p_r = p_r + d_c;
            p_g = p_g + d_c;
            p_b = p_b + d_c;
            p_w = p_w + d_c;

            //Divide to enforce quantization in time
            p_r /= TEMPORAL_DITHER_COUNT;
            p_g /= TEMPORAL_DITHER_COUNT;
            p_b /= TEMPORAL_DITHER_COUNT;
            p_w /= TEMPORAL_DITHER_COUNT;
#endif

            
            //Sometimes the integer math may produce results like 256. Clamp these to 255 to avoid wraparound.
            if (p_b > 255)
                p_b = 255;
            if (p_g > 255)
                p_g = 255;
            if (p_r > 255)
                p_r = 255;
            if (p_w > 255)
                p_w = 255;
            
            _driver.setPixel(uint32_t(_stripadr[strip][led]), p_r, p_g, p_b, p_w);

#if SPATIAL_DITHER_COUNT > 1
            sd_c++;
            if (sd_c >= SPATIAL_DITHER_COUNT)
            {
                sd_c -= SPATIAL_DITHER_COUNT;
            }

#endif
        }
    }
    _driver.showPixels();
    //Cycle Dither Counter
#if TEMPORAL_DITHER_COUNT > 1

    d_c++;
    if (d_c >= TEMPORAL_DITHER_COUNT)
    {
        d_c -= TEMPORAL_DITHER_COUNT;
    }

#endif
}

void _led_set::_allocate_grid()
{
    for (int strip = 0; strip < LED_STRIPS; strip++)
    {
        //Strip is forward allocated
        if (_striplayout[strip].stop > _striplayout[strip].start)
        {
            int stripsize = _striplayout[strip].stop - _striplayout[strip].start + 1;
            for (int pixel = 0; pixel < stripsize; pixel++)
            {
                _stripadr[strip][pixel] = _striplayout[strip].start + pixel;
            }
        }
        //Strip is backward allocated
        else
        {
            int stripsize = _striplayout[strip].start - _striplayout[strip].stop + 1;
            for (int pixel = 0; pixel < stripsize; pixel++)
            {
                _stripadr[strip][pixel] = _striplayout[strip].start - pixel;
            }
        }
    }
}