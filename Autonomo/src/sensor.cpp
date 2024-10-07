#include <Arduino.h>
#include "sensor.hpp"

#pragma region "Sensor Arrela Pinning Macros"
#define sensorF 39
#define sensorD 36
#define sensorE 34
#pragma endregion "Sensor Arrela Pinning Macros"

void sensor_begin()
{
	pinMode(sensorF, INPUT);
	pinMode(sensorD, INPUT);
	pinMode(sensorE, INPUT);
}

void sensor_task(void *pvParameters)
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

void sensor_test()
{
	SerialBT.println("Test Sensors");
	running = true;
	for (;;)
	{
		SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueSharpE, valueSharpF, valueSharpD);
		delay(15);
	}
}

void sensor_test_motor()
{
	digitalWrite(stby, 1);
	SerialBT.println("Test Motors With Sensors");
	running = true;
	for (;;)
	{
		SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueSharpE, valueSharpF, valueSharpD);

		if (valueSharpF)
			forward(255, 255);
		else if (valueSharpE)
			left(80, 80);
		else if (valueSharpD)
			right(80, 80);
		else
			stop();
	}
}
