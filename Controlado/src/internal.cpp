#include <Arduino.h>
#include "internal.hpp"

#pragma region "Internal Arrela Pinning Macros"
#define led 2
#pragma endregion "Internal Arrela Pinning Macros"

uint64_t internal_millis = 0;

void internal_begin()
{
	pinMode(led, OUTPUT);
	internal_millis = internal_end_millis();
}

void internal_led(const bool mode)
{
	digitalWrite(led, static_cast<uint8_t>(mode));
}

void internal_setup_millis()
{
	internal_millis = internal_end_millis();
}

uint64_t internal_begin_millis()
{
	return internal_millis;
}

uint64_t internal_end_millis()
{
	return millis();
}

uint64_t internal_delta_millis()
{
	return internal_end_millis() - internal_begin_millis();
}

void internal_debug(char *out_buffer, const size_t out_size, const char *msg)
{
	snprintf(out_buffer, out_size - 1, "\"%s\" = { begin:%lli; end:%lli; delta:%lli }\n", msg,
				  internal_begin_millis(), internal_end_millis(), internal_delta_millis());
}

void internal_debug(const char *msg)
{
	const size_t buffer_len = 512;
	char buffer[buffer_len];
	internal_debug(buffer, buffer_len, msg);
	Serial.print(buffer);
}