#include <Arduino.h>
#include "sensor.hpp"
#include "engine.hpp"
#include "internal.hpp"

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
		const size_t buffer_len = 512;
		char buffer[buffer_len];
		debug(buffer, buffer_len, sensor, msg);
		Serial.print(buffer);
	}
}