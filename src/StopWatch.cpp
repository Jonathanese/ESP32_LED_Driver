//
//
//

#include "StopWatch.h"

StopWatch::StopWatch()
{
	reset();
	start();
}

StopWatch::StopWatch(bool started)
{
	reset();
	started ? start() : stop();
}

StopWatch::~StopWatch()
{
}

void StopWatch::reset()
{
	time = millis();
}

void StopWatch::reset(unsigned long startTime)
{
	time = millis() - startTime;
}

unsigned long StopWatch::getTime()
{
	return millis() - time;
}

bool StopWatch::start()
{
	if (!isRunning)
	{
		isRunning = true;
		reset();
		return true;
	}
	return false;
}

bool StopWatch::stop()
{
	if (isRunning)
	{
		isRunning = false;
		return true;
	}
	return false;
}

bool StopWatch::passed(unsigned long compareTime)
{
	return (getTime() > compareTime);
}

bool StopWatch::repeat(unsigned long compareTime)
{
	if (passed(compareTime))
	{
		reset();
		return true;
	}
	return false;
}