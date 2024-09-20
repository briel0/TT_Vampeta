#include <Arduino.h>
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

void engine_move(engine_t engine_left, engine_t engine_right)
{
	if (engine_left.direction == BACK)
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

	if (engine_right.direction == BACK)
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
	engine_move(FRONT_STOP, FRONT_STOP);
}