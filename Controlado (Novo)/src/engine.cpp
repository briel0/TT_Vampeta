#include "engine.hpp"

void engine_begin()
{
	pinMode(pwmB, OUTPUT);
	pinMode(b1, OUTPUT);
	pinMode(b2, OUTPUT);
	pinMode(stby, OUTPUT);
	pinMode(a1, OUTPUT);
	pinMode(a2, OUTPUT);
	pinMode(pwmA, OUTPUT);
	digitalWrite(stby, 1);
}

void engine_move(byte sentido_esquerda, byte velocidade_esquerda, byte sentido_direita, byte velocidade_direita)
{
	if (sentido_esquerda == TRAS)
	{
		digitalWrite(a1, 1);
		digitalWrite(a2, 0);
		analogWrite(pwmA, velocidade_esquerda);
	}
	else
	{
		digitalWrite(a1, 0);
		digitalWrite(a2, 1);
		analogWrite(pwmA, velocidade_esquerda);
	}

	if (sentido_direita == TRAS)
	{
		digitalWrite(b1, 1);
		digitalWrite(b2, 0);
		analogWrite(pwmB, velocidade_direita);
	}
	else
	{
		digitalWrite(b1, 0);
		digitalWrite(b2, 1);
		analogWrite(pwmB, velocidade_direita);
	}
}