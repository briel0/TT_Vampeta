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
}