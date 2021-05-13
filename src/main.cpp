#include <Arduino.h>
#include "DebugMessage.h"
#include "PerfMon.h"
#include "Render.h"

PerfMon PM(&Debug, 1000);

Color Pixels[240];

StopWatch tasktimer;
StopWatch frametimer;

void task();

void setup()
{
	Debug.setBaud(921600);
	Debug.setANSI(false);
	Debug.begin();
	LEDS.Init();

	LEDS.setBrightness(16);
	for (int strip = 0; strip < GRID_WIDTH; strip++)
	{
		for (int led = 0; led < GRID_LENGTH; led++)
		{

			LEDS.Pixels[strip][led] = Color(45, 45, 45);
		}
	}
}

void loop()
{
	if (tasktimer.repeat(10))
	{
		task();
	}
	LEDS.Show();
	//delay(20);
	PM.handler();
}

void task()
{
	
	static int val = 255;
	for (int strip = 0; strip < GRID_WIDTH; strip++)
	{
		for (int led = 0; led < GRID_LENGTH; led++)
		{

			LEDS.Pixels[strip][led] = Color(val, val, val);
		}
	}
	val--;
	if(val < 0)
	{
		val = 255;
	}
}