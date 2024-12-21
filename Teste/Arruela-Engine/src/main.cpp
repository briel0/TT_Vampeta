#include <Arduino.h>
#include <engine.hpp>
#include <internal.hpp>
#include <receiver.hpp>
#include <sensor.hpp>
#include <serial.hpp>
#include "main.hpp"

#pragma region "Main Setup"
void setup()
{
	Serial.begin(115200);
	Serial.println("Serial 115200!");

	tt::internal::setup();
	Serial.println("Setup Internal!");

	tt::engine::setup();
	Serial.println("Setup Engine!");

	tt::internal::set_led(false);
	tt::engine::set_standby(false);

	tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(255));
}
#pragma endregion "Main Setup"

#pragma region "Main Loop"
void loop()
{
	vTaskDelay(1);
}
#pragma endregion "Main Loop"