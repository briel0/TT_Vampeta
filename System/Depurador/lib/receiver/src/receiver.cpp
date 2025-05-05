#include <Arduino.h>
#include <IRremote.hpp>
#include "receiver.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#pragma region "Receiver Pinning Macros"
#define IR 13
#pragma endregion "Receiver Pinning Macros"

namespace tt::receiver
{
	void setup()
	{
		IrReceiver.begin(IR, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
		printActiveIRProtocols(&Serial);
	}

	bool decode()
	{
		return IrReceiver.decode();
	}

	void resume()
	{
		IrReceiver.resume();
	}

	uint16_t command()
	{
		return IrReceiver.decodedIRData.command;
	}

	bool update()
	{
		if (decode())
		{
			resume();
			return true;
		}
		return false;
	}

	receiver_t receiver()
	{
		switch (command())
		{
		case static_cast<uint16_t>(receiver_t::test):
			return receiver_t::test;
			break;
		case static_cast<uint16_t>(receiver_t::begin):
			return receiver_t::begin;
			break;
		case static_cast<uint16_t>(receiver_t::end):
			return receiver_t::end;
			break;
		default:
			return receiver_t::none;
			break;
		}
	}

	bool signal(receiver_t sig)
	{
		update();
		return receiver() == sig;
	}

	void debug(char *out_buffer, const size_t out_size, receiver_t receiver, const char *msg)
	{
		char *rec;
		switch (receiver)
		{
		case receiver_t::test:
			rec = const_cast<char *>("test");
			break;
		case receiver_t::begin:
			rec = const_cast<char *>("begin");
			break;
		case receiver_t::end:
			rec = const_cast<char *>("end");
			break;
		default:
			rec = const_cast<char *>("none");
			break;
		}
		snprintf(out_buffer, out_size - 1, "\"%s\" = { command:%i; receiver:\"%s\" }\n", msg, static_cast<uint16_t>(receiver), rec);
	}

	void debug(receiver_t receiver, const char *msg)
	{
		const size_t buffer_len = 512;
		char buffer[buffer_len];
		debug(buffer, buffer_len, receiver, msg);
		Serial.print(buffer);
	}
}