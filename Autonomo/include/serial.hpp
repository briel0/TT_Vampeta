#include <stdint.h>

void serial_begin(const char *name);
int serial_available();
void serial_end();

int serial_read();

void serial_write(uint8_t c);
void serial_printf(const char *fmt, ...);
template <typename T>
void serial_println(T val);
template <typename T>
void serial_print(T val);