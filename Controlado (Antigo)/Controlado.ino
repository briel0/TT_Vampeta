#include <Arduino.h>
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

#define pwmB 4
#define b1 18
#define b2 19
#define stby 5
#define a1 16
#define a2 17
#define pwmA 23

#define led 2
#define IR 15

char SentidoE = 'F';
char SentidoD = 'F';

float VelocidadeE = 0;
float VelocidadeD = 0;

bool L2 = false;
bool R2 = false;

void setup()
{
	pinMode(pwmB, OUTPUT);
	pinMode(b1, OUTPUT);
	pinMode(b2, OUTPUT);
	pinMode(stby, OUTPUT);
	pinMode(a1, OUTPUT);
	pinMode(a2, OUTPUT);
	pinMode(pwmA, OUTPUT);
	digitalWrite(stby, 1);

	Serial.begin(115200);
	PS4.begin("08:d1:f9:c8:f5:3c");
	Serial.println("Ready.");
}

void move(char SE, int VE, char SD, int VD)
{
	/*
		variaveis com final D sao do motor direito e com final E sao do esquerdo
		S = qual sentido de rotação
		'F' -> frente || 'T' -> tras
		V = velocidade de rotação 0 ~ 255
	*/

	if (SE == 'T')
	{
		digitalWrite(a1, 1);
		digitalWrite(a2, 0);
		analogWrite(pwmA, VE);
	}
	else
	{
		digitalWrite(a1, 0);
		digitalWrite(a2, 1);
		analogWrite(pwmA, VE);
	}

	if (SD == 'T')
	{
		digitalWrite(b1, 1);
		digitalWrite(b2, 0);
		analogWrite(pwmB, VD);
	}
	else
	{
		digitalWrite(b1, 0);
		digitalWrite(b2, 1);
		analogWrite(pwmB, VD);
	}
}

void loop()
{
	digitalWrite(2, HIGH);
	if (!PS4.isConnected())
	{
		digitalWrite(2, LOW);
		move('F', 0, 'F', 0);
		return;
	}

	SentidoE = 'F';
	SentidoD = 'F';
	// VELOCIDADES PARA RODAR NO PROPRIO EIXO
	VelocidadeE = 120;
	VelocidadeD = 120;

	if (PS4.Cross() > 0)
	{ // atacar
		while (true)
		{
			move('F', 255, 'F', 235);
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
			SentidoE = 'T';
			SentidoD = 'T';
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
			SentidoE = 'T';
			SentidoD = 'F';
		}
		else if (PS4.LStickX() > 50)
		{
			SentidoE = 'F';
			SentidoD = 'T';
		}
		else
		{
			VelocidadeE = 0;
			VelocidadeD = 0;
		}

		Serial.printf("L2:%i R2:%i VelE:%f VelD:%f\n", L2, R2, VelocidadeE, VelocidadeD);
		move(SentidoE, VelocidadeE, SentidoD, VelocidadeD);
	}
}
