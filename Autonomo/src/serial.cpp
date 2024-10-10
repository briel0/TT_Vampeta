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

void serial_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SerialBT.printf(fmt, args);
	va_end(args);
}

int serial_read()
{
	return SerialBT.read();
}

int serial_available()
{
	return SerialBT.available();
}

void serial_end()
{
	SerialBT.end();
}

/*
template <typename T>
void serial_println(T val)
{
	SerialBT.println(val);
}

template <typename T>
void serial_print(T val)
{
	SerialBT.print(val);
}
*/

void serial_println(const char *val)
{
	SerialBT.println(val);
}

void serial_println(char val)
{
	SerialBT.println(val);
}

void serial_print(const char *val)
{
	SerialBT.print(val);
}

void serial_print(char val)
{
	SerialBT.print(val);
}