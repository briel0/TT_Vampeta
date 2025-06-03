#include <Arduino.h>
#include <BluetoothSerial.h>
#include "serial.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#pragma region "Size Data Defines"
#ifndef BYTE_SIZE
#define BYTE_SIZE 256
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE (BYTE_SIZE * 2)
#endif
#ifndef STACK_SIZE
#define STACK_SIZE (BYTE_SIZE * 16)
#endif
#pragma endregion "Size Data Defines"

namespace tt::serial
{
	BluetoothSerial SerialBT;
	bool enable = false;

	void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
	{
		switch (event)
		{
		case ESP_SPP_SRV_OPEN_EVT:
			enable = true;
			Serial.println("SmartPhone Connected!");
			break;
		case ESP_SPP_CLOSE_EVT:
			enable = false;
			Serial.println("SmartPhone Desconnected!");
			break;
		}
	}

	void setup(const char *name)
	{
		SerialBT.begin(name);
		SerialBT.register_callback(callback);
	}

	bool enabled()
	{
		return enable;
	}

	int available()
	{
		if (!enabled())
		{
			return 0;
		}
		return SerialBT.available();
	}

	void wait()
	{
		if (!enabled())
		{
			return;
		}

		while (!available())
		{
			vTaskDelay(1);
		}
	}

	void erase()
	{
		SerialBT.readStringUntil('\n');
	}

	void end()
	{
		if (!enabled())
		{
			return;
		}
		SerialBT.end();
	}

	int read()
	{
		if (!enabled())
		{
			return -1;
		}
		return SerialBT.read();
	}

	void write(uint8_t c)
	{
		if (!enabled())
		{
			return;
		}
		SerialBT.write(c);
	}

	void flush()
	{
		if (!enabled())
		{
			return;
		}
		SerialBT.flush();
	}

	void ansi_cmd(const char *cmd)
	{
		if (!enabled())
		{
			return;
		}
		printf(SCAPE_CMD("%s"), cmd);
	}

	void color_cmd(const char *cmd)
	{
		if (!enabled())
		{
			return;
		}
		printf(COLOR_CMD("%s"), cmd);
	}

	void clear()
	{
		if (!enabled())
		{
			return;
		}
		ansi_cmd("2J");
	}

	void printf(const char *fmt, ...)
	{
		if (!enabled())
		{
			return;
		}

		const size_t buffer_len = 512;
		char buffer[buffer_len];
		va_list args;

		va_start(args, fmt);
		vsnprintf(buffer, buffer_len - 1, fmt, args);
		SerialBT.print(buffer);
		va_end(args);
	}

	void scanf(const char *fmt, ...)
	{
		if (!enabled())
		{
			return;
		}

		String in = SerialBT.readStringUntil('\n');
		va_list args;

		va_start(args, fmt);
		vsscanf(in.c_str(), fmt, args);
		va_end(args);
	}

	String input()
	{
		if (!enabled())
		{
			return String("");
		}

		return SerialBT.readStringUntil('\n');
	}

	template <typename T>
	void println(T val)
	{
		if (!enabled())
		{
			return;
		}
		SerialBT.println(val);
	}
	template void println(int8_t);
	template void println(int16_t);
	template void println(int32_t);
	template void println(int64_t);
	template void println(uint8_t);
	template void println(uint16_t);
	template void println(uint32_t);
	template void println(uint64_t);
	template void println(long);
	template void println(unsigned long);
	template void println(float);
	template void println(double);
	template void println(char);
	template void println(char *);
	template void println(const char *);
	template void println(String);

	template <typename T>
	void print(T val)
	{
		if (!enabled())
		{
			return;
		}
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
	template void print(long);
	template void print(unsigned long);
	template void print(float);
	template void print(double);
	template void print(char);
	template void print(char *);
	template void print(const char *);
	template void print(String);
}