#include <cstdint>
#include <cstddef>

#pragma region "Internal Macros"
#define TT_INTERNAL_BETWEEN(x, min, max) ((x >= min && x <= max) * (x) + (min) * (x < min) + (max) * (x > max))
#pragma endregion "Internal Macros"

namespace tt
{
	namespace internal
	{
		void setup();
		bool get_led();
		void set_led(const bool mode);
		void setup_millis();
		uint64_t begin_millis();
		uint64_t end_millis();
		uint64_t delta_millis();
		void setup_micros();
		uint64_t begin_micros();
		uint64_t end_micros();
		uint64_t delta_micros();
		const char *mac_address();
		void debug(char *out_buffer, const size_t out_size, const char *msg);
		void debug(const char *msg);
	}
}