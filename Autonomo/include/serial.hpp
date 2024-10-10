#include <stdint.h>

void serial_begin(const char *name);
void serial_printf(const char *fmt, ...);
int serial_read();
int serial_available();
void serial_end();

void serial_println(const char *val);
void serial_println(char val);
void serial_print(const char *val);
void serial_print(char val);