#include <Arduino.h>
#include "engine.hpp"
#include "sensor.hpp"

engine_t engine_left = ENGINE_FRONT_STOP;
engine_t engine_right = ENGINE_FRONT_STOP;

void setup()
{
	Serial.begin(115200);
	engine_begin();
	Serial.println("Ready.");
}

void loop()
{
}