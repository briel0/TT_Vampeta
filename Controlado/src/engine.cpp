#include <Arduino.h>
#include "engine.hpp"

#pragma region "Engine Arrela Pinning Macros"
#define pwmA 23
#define a1 16
#define a2 17
#define pwmB 4
#define b1 18
#define b2 19
#define stby 5
#define led 2
#pragma endregion "Engine Arrela Pinning Macros"

void engine_begin()
{
	pinMode(pwmA, OUTPUT);
	pinMode(a1, OUTPUT);
	pinMode(a2, OUTPUT);
	pinMode(pwmB, OUTPUT);
	pinMode(b1, OUTPUT);
	pinMode(b2, OUTPUT);
	pinMode(stby, OUTPUT);
	pinMode(led, OUTPUT);

	digitalWrite(stby, HIGH);
}

void engine_alive()
{
	digitalWrite(led, HIGH);
}

void engine_kill()
{
	digitalWrite(led, LOW);
	engine_stop();
}

void engine_move(engine_t engine_left, engine_t engine_right)
{
	if (engine_left.direction == ENGINE_DIRECTION_BACK)
	{
		digitalWrite(a1, HIGH);
		digitalWrite(a2, LOW);
		analogWrite(pwmA, engine_left.speed);
	}
	else
	{
		digitalWrite(a1, LOW);
		digitalWrite(a2, HIGH);
		analogWrite(pwmA, engine_left.speed);
	}

	if (engine_right.direction == ENGINE_DIRECTION_BACK)
	{
		digitalWrite(b1, HIGH);
		digitalWrite(b2, LOW);
		analogWrite(pwmB, engine_right.speed);
	}
	else
	{
		digitalWrite(b1, LOW);
		digitalWrite(b2, HIGH);
		analogWrite(pwmB, engine_right.speed);
	}
}

void engine_stop()
{
	digitalWrite(a1, HIGH);
	digitalWrite(a2, HIGH);
	digitalWrite(b1, HIGH);
	digitalWrite(b2, HIGH);
}