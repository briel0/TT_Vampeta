#include <stdint.h>

void internal_begin();
void internal_led(const bool mode);
void internal_setup_millis();
uint64_t internal_begin_millis();
uint64_t internal_end_millis();
uint64_t internal_delta_millis();