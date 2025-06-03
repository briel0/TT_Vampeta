#include <Arduino.h>
#include "sensor.hpp"
#include "engine.hpp"
#include "internal.hpp"

#pragma region "Size Data Defines"
#ifndef BYTE_SIZE
#define BYTE_SIZE 256
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE (BYTE_SIZE * 2)
#endif
#ifndef STACK_SIZE
#define STACK_SIZE (BYTE_SIZE * 16)
#endif
#pragma endregion "Size Data Defines"

#pragma region "Sensor Pinning Macros"
#define sensor_front 39
#define sensor_right 36
#define sensor_left 34
#pragma endregion "Sensor Pinning Macros"

namespace tt::sensor
{
	void setup()
	{
		pinMode(sensor_left, INPUT);
		pinMode(sensor_front, INPUT);
		pinMode(sensor_right, INPUT);
	}

	sensor_t create_snapshot()
	{
		sensor_t sensor;
		sensor.left = digitalRead(sensor_left);
		sensor.front = digitalRead(sensor_front);
		sensor.right = digitalRead(sensor_right);
		return sensor;
	}

	void debug(char *out_buffer, const size_t out_size, sensor_t sensor, const char *msg)
	{
		snprintf(out_buffer, out_size - 1, "\"%s\" = { left:%i; front:%i; right:%i }\n", msg, sensor.left, sensor.front, sensor.right);
	}

	void debug(sensor_t sensor, const char *msg)
	{
		char buffer[BUFFER_SIZE];
		debug(buffer, BUFFER_SIZE, sensor, msg);
		Serial.print(buffer);
	}
}