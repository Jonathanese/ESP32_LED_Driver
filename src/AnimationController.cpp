#include "AnimationController.h"

_animation_controller AnimationController;

void _animation_controller::init()
{
    LEDSet.Init();
    _mqtt_client.setCallback(MQTT_Update_callback);
    TransitionTo(_current_animation, DEFAULT_TRANSITION_TIME, 255);
}
void _animation_controller::loop()
{
    if (_frameTimer.repeat(_frame_time))
    {
        _frame_loop();
    }
    LEDSet.Show();
    MQTT::loop();
}

void _animation_controller::_frame_loop()
{
    _current_animation.animator();
    //Before we show, check if we are transitioning, and perform the transition.
    _transition_frame();
}

void _animation_controller::_transition_frame()
{
    if (_transition_frames_remaining > 0)
    {
        uint8_t lerp_amt = (uint16_t)_transition_frames_remaining * 255 / _transition_frames_total;
        for (uint8_t strip = 0; strip < GRID_WIDTH; strip++)
        {
            for (uint8_t led = 0; led < GRID_LENGTH; led++)
            {
                LEDSet.Pixels[strip][led].Lerp(_back_pixels[strip][led], lerp_amt);
            }
        }
        LEDSet.setBrightness(tools::lerp8(_brightness, _old_brightness, lerp_amt));

        _transition_frames_remaining--;
    }
}

void _animation_controller::TransitionTo(Animation new_animation, uint16_t transition_time, uint8_t brightness)
{
    //Copy current frame to _back_pixels so we have colors to transition from.
    memcpy(_back_pixels, LEDSet.Pixels, sizeof(Pixel) * GRID_WIDTH * GRID_LENGTH);
    setFrameRate(new_animation.getFrameRate());
    uint8_t transition_frames = transition_time / _frame_time;
    _transition_frames_remaining = transition_frames;
    _transition_frames_total = transition_frames;
    _old_brightness = _brightness;
    _brightness = brightness;
    

    //Switch over to the new animation
    _current_animation = new_animation;
}

void _animation_controller::TransitionTo(String animation_name, uint16_t transition_time, uint8_t brightness, Color foreground)
{
    TransitionTo(Animation::getAnimation(animation_name), transition_time, brightness);
    Foreground = foreground;
}

void _animation_controller::setFrameRate(uint8_t fps)
{
    _frame_time = uint8_t(1000.0 / fps);
}

void _animation_controller::ParseJson(char *topic, uint8_t *payload, unsigned int length)
{
    deserializeJson(_json_doc, payload);
    bool isChange = false;
    uint16_t transition_time = DEFAULT_TRANSITION_TIME;
    Color FG = Foreground;
    uint8_t brightness = _brightness;
    String animation = _current_animation.getName();
    if(animation == "")
    {
        animation = "none";
    }

    if (_json_doc.containsKey("color"))
    {
        
        FG.r = _json_doc["color"]["r"];
        FG.g = _json_doc["color"]["g"];
        FG.b = _json_doc["color"]["b"];
        if (FG.rgb != Foreground.rgb)
        {
            Foreground = FG;
            isChange = true;
        }
        Debug.Message(DM_INFO, "Received Color: " + String(FG.r) + " " + String(FG.g) + " " + String(FG.b));
    }

    if (_json_doc.containsKey("brightness"))
    {
       
        brightness = _json_doc["brightness"];
        if (brightness != _brightness)
        {
            isChange = true;
        }
         Debug.Message(DM_INFO, "Received Brightness: " + String(brightness));
    }

    if (_json_doc.containsKey("effect"))
    {
        
        animation = (const char *)_json_doc["effect"];
        if (animation != _current_animation.getName())
        {
            isChange = true;
        }
        Debug.Message(DM_INFO, "Received Animation: " + animation);
    }

    if (_json_doc.containsKey("transition"))
    {
        transition_time = _json_doc["transition"];
        transition_time *= 1000;
        
    }

    if (_json_doc.containsKey("state"))
    {
        String st = _json_doc["state"];
        if(st == "OFF")
        {
            animation = "OFF";
            isChange = true;
        }
        if(st == "ON" && !_json_doc.containsKey("effect"))
        {
            animation = "none";
            isChange = true;
        }
    }

    if (isChange)
    {
        TransitionTo(animation, transition_time, brightness, FG);
        _send_state();
    }
}

void _animation_controller::_send_state()
{
    _json_doc.clear();
    if (_current_animation.getName() == "OFF")
    {
        _json_doc["state"] = "OFF";
    }
    else
    {
        _json_doc["state"] = "ON";
        JsonObject color = _json_doc.createNestedObject("color");
        color["r"] = Foreground.r;
        color["g"] = Foreground.g;
        color["b"] = Foreground.b;

        _json_doc["brightness"] = _brightness;
        _json_doc["effect"] = _current_animation.getName();
    }

    //This part is dumb. I have to use a string for SerializeJson, but need a const char* for publish

    String buffer;

    serializeJson(_json_doc, buffer);

    char *charbuffer = new char[buffer.length() + 1];

    buffer.toCharArray(charbuffer, buffer.length() + 1);

    MQTT::publish(SECRET_MQTT_STATE_TOPIC, charbuffer);

    delete charbuffer;
}

static void MQTT_Update_callback(char *topic, uint8_t *payload, unsigned int length)
{
    AnimationController.ParseJson(topic, payload, length);
}
