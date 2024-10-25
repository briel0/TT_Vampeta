#include <Arduino.h>
#include "sensor.hpp"
#include "engine.hpp"
#include "internal.hpp"

#pragma region "Sensor Pinning Macros"
#define sensor_front 39
#define sensor_right 36
#define sensor_left 34
#pragma endregion "Sensor Pinning Macros"

void sensor_begin()
{
	pinMode(sensor_left, INPUT);
	pinMode(sensor_front, INPUT);
	pinMode(sensor_right, INPUT);
}

sensor_t sensor_create_snapshot()
{
	sensor_t sensor;
	sensor.left = digitalRead(sensor_left);
	sensor.front = digitalRead(sensor_front);
	sensor.right = digitalRead(sensor_right);
	return sensor;
}

void sensor_debug(sensor_t sensor, const char *msg)
{
	Serial.printf("\"%s\" = { left:%i; front:%i; right:%i }\n", msg, sensor.left, sensor.front, sensor.right);
}