#include <Arduino.h>
#include "internal.hpp"

#pragma region "Internal Arrela Pinning Macros"
#define led 2
#pragma endregion "Internal Arrela Pinning Macros"

void internal_begin()
{
	pinMode(led, OUTPUT);
}

void internal_led(bool mode)
{
	digitalWrite(led, static_cast<uint8_t>(mode));
}