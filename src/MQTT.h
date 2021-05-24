#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "StopWatch.h"
#include "DebugMessage.h"
#include "CONFIG.h"

extern PubSubClient _mqtt_client;

namespace MQTT
{
    void loop();
    void publish(const char* topic, const char* payload);
}