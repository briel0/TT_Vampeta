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
}