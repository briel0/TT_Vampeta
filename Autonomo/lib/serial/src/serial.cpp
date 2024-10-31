#include <Arduino.h>
#include <BluetoothSerial.h>
#include "serial.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

namespace tt::serial
{
	BluetoothSerial SerialBT;

	void setup(const char *name)
	{
		SerialBT.begin(name);
	}

	int available()
	{
		return SerialBT.available();
	}

	void end()
	{
		SerialBT.end();
	}

	int read()
	{
		return SerialBT.read();
	}

	void write(uint8_t c)
	{
		SerialBT.write(c);
	}

	void printf(const char *fmt, ...)
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
	void println(T val)
	{
		SerialBT.println(val);
	}
	template void println(int16_t);
	template void println(int32_t);
	template void println(int8_t);
	template void println(int64_t);
	template void println(uint8_t);
	template void println(uint16_t);
	template void println(uint32_t);
	template void println(uint64_t);
	template void println(float);
	template void println(double);
	template void println(char);
	template void println(const char *);

	template <typename T>
	void print(T val)
	{
		SerialBT.print(val);
	}
	template void print(int8_t);
	template void print(int16_t);
	template void print(int32_t);
	template void print(int64_t);
	template void print(uint8_t);
	template void print(uint16_t);
	template void print(uint32_t);
	template void print(uint64_t);
	template void print(float);
	template void print(double);
	template void print(char);
	template void print(const char *);
}