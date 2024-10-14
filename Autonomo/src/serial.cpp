#include <Arduino.h>
#include <BluetoothSerial.h>
#include "serial.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void serial_begin(const char *name)
{
	SerialBT.begin(name);
}

int serial_available()
{
	return SerialBT.available();
}

void serial_end()
{
	SerialBT.end();
}

int serial_read()
{
	return SerialBT.read();
}

void serial_write(uint8_t c)
{
	SerialBT.write(c);
}

void serial_printf(const char *fmt, ...)
{
	const size_t buffer_len = 512;
	char buffer[buffer_len];
	va_list args;

	va_start(args, fmt);
	vsnprintf(buffer, buffer_len - 1, fmt, args);
	SerialBT.print(buffer);
	va_end(args);
}

template <typename T>
void serial_println(T val)
{
	SerialBT.println(val);
}
template void serial_println(int8_t);
template void serial_println(int16_t);
template void serial_println(int32_t);
template void serial_println(int64_t);
template void serial_println(uint8_t);
template void serial_println(uint16_t);
template void serial_println(uint32_t);
template void serial_println(uint64_t);
template void serial_println(float);
template void serial_println(double);
template void serial_println(char);
template void serial_println(const char *);

template <typename T>
void serial_print(T val)
{
	SerialBT.print(val);
}
template void serial_print(int8_t);
template void serial_print(int16_t);
template void serial_print(int32_t);
template void serial_print(int64_t);
template void serial_print(uint8_t);
template void serial_print(uint16_t);
template void serial_print(uint32_t);
template void serial_print(uint64_t);
template void serial_print(float);
template void serial_print(double);
template void serial_print(char);
template void serial_print(const char *);