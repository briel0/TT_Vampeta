#include <Arduino.h>
#include "internal.hpp"

#pragma region "Internal Pinning Macros"
#define led 2
#pragma endregion "Internal Pinning Macros"

namespace tt::internal
{
	uint64_t internal_millis = 0;
	bool led_mode = false;

	void setup()
	{
		pinMode(led, OUTPUT);
		internal_millis = end_millis();
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

	void debug(char *out_buffer, const size_t out_size, const char *msg)
	{
		snprintf(out_buffer, out_size - 1, "\"%s\" = { begin:%lli; end:%lli; delta:%lli }\n", msg,
				 begin_millis(), end_millis(), delta_millis());
	}

	void debug(const char *msg)
	{
		const size_t buffer_len = 512;
		char buffer[buffer_len];
		debug(buffer, buffer_len, msg);
		Serial.print(buffer);
	}
}