#include <stdint.h>

void receiver_begin();
bool receiver_decode();
void receiver_resume();
uint16_t receiver_command();