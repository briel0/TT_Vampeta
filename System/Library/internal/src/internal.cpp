#include <Arduino.h>
#include <WiFi.h>
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

#pragma region "Internal Pinning Macros"
#define led 2
#pragma endregion "Internal Pinning Macros"

namespace tt::internal
{
	char wifi_mac_address[32] = {};
	uint64_t internal_millis = 0;
	uint64_t internal_micros = 0;
	bool led_mode = false;

	void setup()
	{
		pinMode(led, OUTPUT);
		setup_millis();
		setup_micros();
		WiFi.macAddress().toCharArray(wifi_mac_address, sizeof(wifi_mac_address));
	}

	bool get_led()
	{
		return led_mode;
	}

	void set_led(const bool mode)
	{
		if (led_mode != mode)
		{
			led_mode = mode;
			digitalWrite(led, static_cast<uint8_t>(led_mode));
		}
	}

	void setup_millis()
	{
		internal_millis = end_millis();
	}

	uint64_t begin_millis()
	{
		return internal_millis;
	}

	uint64_t end_millis()
	{
		return millis();
	}

	uint64_t delta_millis()
	{
		return end_millis() - begin_millis();
	}

	void setup_micros()
	{
		internal_micros = end_micros();
	}

	uint64_t begin_micros()
	{
		return internal_micros;
	}

	uint64_t end_micros()
	{
		return micros();
	}

	uint64_t delta_micros()
	{
		return end_micros() - begin_micros();
	}

	const char *mac_address()
	{
		return wifi_mac_address;
	}

	void debug(char *out_buffer, const size_t out_size, const char *msg)
	{
		snprintf(out_buffer, out_size - 1, "\"%s\" = { begin:%lli; end:%lli; delta:%lli }\n", msg,
				 begin_millis(), end_millis(), delta_millis());
	}

	void debug(const char *msg)
	{
		char buffer[BUFFER_SIZE];
		debug(buffer, BUFFER_SIZE, msg);
		Serial.print(buffer);
	}
}