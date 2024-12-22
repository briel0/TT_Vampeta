#include <Arduino.h>
#include "serial.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

namespace tt::serial
{
	BluetoothSerial SerialBT;
	bool is_enable = false;

	void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
	{
		switch (event)
		{
		case ESP_SPP_SRV_OPEN_EVT:
			is_enable = true;
			Serial.println("SmartPhone Connected!");
			break;
		case ESP_SPP_CLOSE_EVT:
			is_enable = false;
			Serial.println("SmartPhone Desconnected!");
			break;
		}
	}

	void setup(const char *name)
	{
		SerialBT.begin(name);
		SerialBT.register_callback(callback);
	}

	BluetoothSerial serial()
	{
		return SerialBT;
	}

	bool enable()
	{
		return is_enable;
	}

	int available()
	{
		if (!enable())
		{
			return 0;
		}
		return SerialBT.available();
	}

	void end()
	{
		if (!enable())
		{
			return;
		}
		SerialBT.end();
	}

	int read()
	{
		if (!enable())
		{
			return -1;
		}
		return SerialBT.read();
	}

	void write(uint8_t c)
	{
		if (!enable())
		{
			return;
		}
		SerialBT.write(c);
	}

	void printf(const char *fmt, ...)
	{
		if (!enable())
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
}