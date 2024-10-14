#include <stdint.h>

#pragma region "Internal Macros"
#define INTERNAL_BETWEEN(x, min, max) ((x >= min && x <= max) * (x) + (min) * (x < min) + (max) * (x > max))
#pragma endregion "Internal Macros"

void internal_begin();
void internal_led(const bool mode);
void internal_setup_millis();
uint64_t internal_begin_millis();
uint64_t internal_end_millis();
uint64_t internal_delta_millis();
void internal_debug(char *out_buffer, const size_t out_size, const char *msg);
void internal_debug(const char *msg);