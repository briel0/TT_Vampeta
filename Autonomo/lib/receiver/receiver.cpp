#include <Arduino.h>
#include <IRremote.hpp>
#include "receiver.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#pragma region "Receiver Pinning Macros"
#define IR 13
#pragma endregion "Receiver Pinning Macros"

void receiver_begin()
{
	IrReceiver.begin(IR, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
	printActiveIRProtocols(&Serial);
}

bool receiver_decode()
{
	return IrReceiver.decode();
}

void receiver_resume()
{
	IrReceiver.resume();
}

uint16_t receiver_command()
{
	return IrReceiver.decodedIRData.command;
}