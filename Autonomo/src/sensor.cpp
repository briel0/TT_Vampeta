#include <Arduino.h>
#include "sensor.hpp"
#include "engine.hpp"
#include "internal.hpp"

#pragma region "Sensor Arrela Pinning Macros"
#define sensor_front 39
#define sensor_right 36
#define sensor_left 34
#pragma endregion "Sensor Arrela Pinning Macros"

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

/*
void sensor_test()
{
	SerialBT.println("Test Sensors");
	running = true;
	for (;;)
	{
		SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueSharpE, valueSharpF, valueSharpD);
		delay(15);
	}
}

void sensor_test_motor()
{
	digitalWrite(stby, 1);
	SerialBT.println("Test Motors With Sensors");
	running = true;
	for (;;)
	{
		SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueSharpE, valueSharpF, valueSharpD);

		if (valueSharpF)
			forward(255, 255);
		else if (valueSharpE)
			left(80, 80);
		else if (valueSharpD)
			right(80, 80);
		else
			stop();
	}
}
*/