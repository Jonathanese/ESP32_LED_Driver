#include <Arduino.h>
#include "DebugMessage.h"
#include "PerfMon.h"
#include "Animation.h"

PerfMon PM(&Debug, 1000);

Color Pixels[240];

StopWatch Timer;
bool TaskSwitch;

void task1();
void task2();

void setup()
{
	Debug.setBaud(921600);
	Debug.setANSI(false);
	Debug.begin();
	LEDSet.Init();
	LEDSet.setBrightness(128);
	AnimationController.TransitionTo(task1,0);
	AnimationController.setFrameRate(30);
}

void loop()
{
	if(Timer.repeat(2000))
	{
		if(TaskSwitch)
		{
			AnimationController.TransitionTo(task1, 30);
			TaskSwitch = false;
		}
		else{
			AnimationController.TransitionTo(task2, 30);
			TaskSwitch = true;
		}
	}
	AnimationController.loop();
	PM.handler();
}

void task1()
{
	Color testcolor(255,0,0);
	testcolor.Lerp(Color(0,255,255), 192);
	for (int strip = 0; strip < GRID_WIDTH; strip++)
	{
		for (int led = 0; led < GRID_LENGTH; led++)
		{

			LEDSet.Pixels[strip][led] = testcolor;
		}
	}
}

void task2()
{

	for (int strip = 0; strip < GRID_WIDTH; strip++)
	{
		for (int led = 0; led < GRID_LENGTH; led++)
		{

			LEDSet.Pixels[strip][led] = Color(0, 0, 255);
		}
	}

}