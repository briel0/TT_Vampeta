#include <Arduino.h>
#include "sensor.hpp"

#pragma region "Sensor Arrela Pinning Macros"
#define sensorF 39
#define sensorD 36
#define sensorE 34
#pragma endregion "Sensor Arrela Pinning Macros"

void sensor_begin()
{
	pinMode(sensorF, INPUT);
	pinMode(sensorD, INPUT);
	pinMode(sensorE, INPUT);
}