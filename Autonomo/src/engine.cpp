#include <Arduino.h>
#include "engine.hpp"

#pragma region "Engine Arrela Pinning Macros"
#define pwmA 4
#define a1 18
#define a2 19
#define pwmB 23
#define b1 16
#define b2 17
#define stby 5
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
}

void engine_standby(const bool mode)
{
	digitalWrite(stby, static_cast<uint8_t>(!mode));
}

void engine_move(const engine_t engine_left, const engine_t engine_right)
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

void engine_debug(char *out_buffer, const size_t out_size, engine_t engine, const char *msg)
{
	snprintf(out_buffer, out_size - 1, "\"%s\" = { direction:%i; speed:%i }\n", msg, engine.direction, engine.speed);
}

void engine_debug(engine_t engine, const char *msg)
{
	const size_t buffer_len = 512;
	char buffer[buffer_len];
	engine_debug(buffer, buffer_len, engine, msg);
	Serial.print(buffer);
}