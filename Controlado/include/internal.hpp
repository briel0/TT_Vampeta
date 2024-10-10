#include <stdint.h>

#define INTERNAL_BETWEEN(x, min, max) ((x >= min && x <= max) * (x) + (min) * (x < min) + (max) * (x > max))

void internal_begin();
void internal_led(const bool mode);
void internal_setup_millis();
uint64_t internal_begin_millis();
uint64_t internal_end_millis();
uint64_t internal_delta_millis();
void internal_debug(const char *msg);