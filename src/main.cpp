#include <Arduino.h>
#include "DebugMessage.h"
#include "PerfMon.h"
#include "Animations.h"
#include "MQTT.h"

PerfMon PM(&Debug, 10000);

void setup()
{
	Debug.setBaud(921600);
	Debug.setANSI(false);
	Debug.begin();
	AnimationController.init();
}

void loop()
{
	AnimationController.loop();
	PM.handler();
}
