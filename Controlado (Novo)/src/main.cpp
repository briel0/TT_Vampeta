#include <Arduino.h>
#include "engine.hpp"
#include "controller.hpp"

#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

byte SentidoE = FRENTE;
byte SentidoD = FRENTE;

float VelocidadeE = 0;
float VelocidadeD = 0;

bool L2 = false;
bool R2 = false;

void setup()
{
	Serial.begin(115200);
	engine_begin();
	controller_begin("08:d1:f9:c8:f5:3c");
	Serial.println("Ready.");
}

void loop()
{
	digitalWrite(2, HIGH);
	if (!PS4.isConnected())
	{
		digitalWrite(2, LOW);
		engine_stop();
		return;
	}

	SentidoE = FRENTE;
	SentidoD = FRENTE;

	// VELOCIDADES PARA RODAR NO PROPRIO EIXO
	VelocidadeE = 120;
	VelocidadeD = 120;

	if (PS4.Cross() > 0)
	{ // atacar
		while (true)
		{
			engine_move(FRENTE, 255, FRENTE, 235);
			if (!PS4.Cross())
			{
				break;
			}
		}
	}
	else
	{
		if (abs(PS4.R2()) > 0)
		{
			R2 = true;
		}

		if (abs(PS4.R2()) == 0)
		{
			R2 = false;
		}

		if (abs(PS4.L2()) > 0)
		{
			L2 = true;
		}

		if (abs(PS4.L2()) == 0)
		{
			L2 = false;
		}

		if (R2)
		{
			VelocidadeE = map(PS4.R2Value(), 0, 255, 0, 140);
			VelocidadeD = map(PS4.R2Value(), 0, 230, 0, 140);

			if (PS4.LStickX() <= -50)
			{
				VelocidadeE *= 2;
				VelocidadeD *= 0.25;
				if (VelocidadeD > 255)
				{
					VelocidadeD = 255;
				}
				if (VelocidadeE > 255)
				{
					VelocidadeE = 255;
				}
			}
			else if (PS4.LStickX() > 50)
			{
				VelocidadeE *= 0.25;
				VelocidadeD *= 2;
				if (VelocidadeD > 255)
				{
					VelocidadeD = 255;
				}
				if (VelocidadeE > 255)
				{
					VelocidadeE = 255;
				}
			}
		}
		else if (L2)
		{
			SentidoE = TRAS;
			SentidoD = TRAS;
			VelocidadeE = map(PS4.L2Value(), 0, 255, 0, 140);
			VelocidadeD = map(PS4.L2Value(), 0, 230, 0, 140);
			if (PS4.LStickX() <= -50)
			{
				VelocidadeE *= 2;
				VelocidadeD *= 0.25;
				if (VelocidadeD > 255)
				{
					VelocidadeD = 255;
				}
				if (VelocidadeE > 255)
				{
					VelocidadeE = 255;
				}
			}
			else if (PS4.LStickX() > 50)
			{
				VelocidadeE *= 0.25;
				VelocidadeD *= 2;
				if (VelocidadeD > 255)
				{
					VelocidadeD = 255;
				}
				if (VelocidadeE > 255)
				{
					VelocidadeE = 255;
				}
			}
		}
		else if (PS4.LStickX() <= -50)
		{
			SentidoE = TRAS;
			SentidoD = FRENTE;
		}
		else if (PS4.LStickX() > 50)
		{
			SentidoE = FRENTE;
			SentidoD = TRAS;
		}
		else
		{
			VelocidadeE = 0;
			VelocidadeD = 0;
		}

		Serial.printf("L2:%i R2:%i VelE:%f VelD:%f\n", L2, R2, VelocidadeE, VelocidadeD);
		engine_move(SentidoE, VelocidadeE, SentidoD, VelocidadeD);
	}
}
