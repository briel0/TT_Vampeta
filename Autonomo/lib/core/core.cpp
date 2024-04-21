#define DECODE_SONY

#include "core.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void Core::FunctionSensorTask(void *pvParameters)
{
	while (true)
	{
		if (running)
		{
			if (IrReceiver.decode())
			{
				IrReceiver.resume();
				if (IrReceiver.decodedIRData.command == 0x2)
				{
					digitalWrite(led, 1);
					stop();
					ESP.restart();
				}
			}

			valueSharpF = digitalRead(sensorF);
			valueSharpD = digitalRead(sensorD);
			valueSharpE = digitalRead(sensorE);

			if (valueSharpE)
			{
				direc = esq;
			}
			else if (valueSharpD)
			{
				direc = dir;
			}
		}
		vTaskDelay(1);
	}
}

void Core::init()
{
}

void Core::close()
{
}

void Core::stop()
{
	digitalWrite(a1, 1);
	digitalWrite(a2, 1);
	digitalWrite(b1, 1);
	digitalWrite(b2, 1);
}

void Core::backward(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 0);
	digitalWrite(b2, 1);
	digitalWrite(a1, 1);
	digitalWrite(a2, 0);
	analogWrite(pwmB, pb);
	analogWrite(pwmA, pa);
}

void Core::forward(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 1);
	digitalWrite(b2, 0);
	digitalWrite(a1, 0);
	digitalWrite(a2, 1);
	analogWrite(pwmB, pb);
	analogWrite(pwmA, pa);
}

void Core::right(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 1);
	digitalWrite(b2, 0);
	digitalWrite(a1, 1);
	digitalWrite(a2, 0);
	analogWrite(pwmB, pb);
	analogWrite(pwmA, pa);
}

void Core::left(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 0);
	digitalWrite(b2, 1);
	digitalWrite(a1, 0);
	digitalWrite(a2, 1);
	analogWrite(pwmB, pb);
	analogWrite(pwmA, pa);
}