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
#define IR 15
#pragma endregion "Engine Arrela Pinning Macros"

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

void engine_move(engine_t engine_left, engine_t engine_right)
{
	if (engine_left.direction == ENGINE_DIRECTION_BACK)
	{
		digitalWrite(a1, 1);
		digitalWrite(a2, 0);
		analogWrite(pwmA, engine_left.speed);
	}
	else
	{
		digitalWrite(a1, 0);
		digitalWrite(a2, 1);
		analogWrite(pwmA, engine_left.speed);
	}

	if (engine_right.direction == ENGINE_DIRECTION_BACK)
	{
		digitalWrite(b1, 1);
		digitalWrite(b2, 0);
		analogWrite(pwmB, engine_right.speed);
	}
	else
	{
		digitalWrite(b1, 0);
		digitalWrite(b2, 1);
		analogWrite(pwmB, engine_right.speed);
	}
}

void engine_stop()
{
	engine_move(ENGINE_FRONT_STOP, ENGINE_FRONT_STOP);
}