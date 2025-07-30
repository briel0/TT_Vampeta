#include <Arduino.h>
#include <utilitie.hpp>
#include "engine.hpp"

#ifndef BYTE_SIZE
#define BYTE_SIZE 256
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE (BYTE_SIZE * 2)
#endif
#ifndef STACK_SIZE
#define STACK_SIZE (BYTE_SIZE * 16)
#endif

#ifndef PWM_A
#define PWM_A 4
#endif
#ifndef A_1
#define A_1 16
#endif
#ifndef A_2
#define A_2 17
#endif
#ifndef PWM_B
#define PWM_B 23
#endif
#ifndef B_1
#define B_1 18
#endif
#ifndef B_2
#define B_2 19
#endif
#ifndef STBY
#define STBY 5
#endif

#define PIN_BOOL(x) ((HIGH) * (x) + (LOW) * !(x))

namespace tt::engine
{
	engine_t current_engine_left = TT_ENGINE_DEFAULT;
	engine_t current_engine_right = TT_ENGINE_DEFAULT;
	bool standby_mode = true;

	void setup()
	{
		pinMode(PWM_A, OUTPUT);
		pinMode(A_1, OUTPUT);
		pinMode(A_2, OUTPUT);
		pinMode(PWM_B, OUTPUT);
		pinMode(B_1, OUTPUT);
		pinMode(B_2, OUTPUT);
		pinMode(STBY, OUTPUT);
		set_standby(true);
	}

	bool get_standby()
	{
		return standby_mode;
	}

	void set_standby(const bool mode)
	{
		standby_mode = mode;
		digitalWrite(STBY, static_cast<uint8_t>(!standby_mode));
	}

	void init()
	{
		set_standby(false);

		current_engine_left = TT_ENGINE_DEFAULT;
		digitalWrite(A_1, PIN_BOOL(current_engine_left.direction == TT_ENGINE_DIRECTION_BACK));
		digitalWrite(A_2, PIN_BOOL(current_engine_left.direction == TT_ENGINE_DIRECTION_FRONT));
		analogWrite(PWM_A, current_engine_left.speed);

		current_engine_right = TT_ENGINE_DEFAULT;
		digitalWrite(B_1, PIN_BOOL(current_engine_right.direction == TT_ENGINE_DIRECTION_BACK));
		digitalWrite(B_2, PIN_BOOL(current_engine_right.direction == TT_ENGINE_DIRECTION_FRONT));
		analogWrite(PWM_B, current_engine_right.speed);
	}

	void move(const engine_t engine_left, const engine_t engine_right)
	{
		if (current_engine_left == engine_left && current_engine_right == engine_right)
		{
			return;
		}

		current_engine_left = engine_left;
		digitalWrite(A_1, PIN_BOOL(current_engine_left.direction == TT_ENGINE_DIRECTION_BACK));
		digitalWrite(A_2, PIN_BOOL(current_engine_left.direction == TT_ENGINE_DIRECTION_FRONT));
		analogWrite(PWM_A, current_engine_left.speed);

		current_engine_right = engine_right;
		digitalWrite(B_1, PIN_BOOL(current_engine_right.direction == TT_ENGINE_DIRECTION_BACK));
		digitalWrite(B_2, PIN_BOOL(current_engine_right.direction == TT_ENGINE_DIRECTION_FRONT));
		analogWrite(PWM_B, current_engine_right.speed);
	}

	void stop()
	{
		digitalWrite(A_1, HIGH);
		digitalWrite(A_2, HIGH);
		digitalWrite(B_1, HIGH);
		digitalWrite(B_2, HIGH);

		current_engine_left = TT_ENGINE_FRONT_STOP;
		current_engine_right = TT_ENGINE_FRONT_STOP;
	}

	void debug(char *out_buffer, const size_t out_size, engine_t engine, const char *msg)
	{
		snprintf(out_buffer, out_size - 1, "\"%s\" = { direction:%i; speed:%i }\n", msg, engine.direction, engine.speed);
	}

	void debug(engine_t engine, const char *msg)
	{
		char buffer[BUFFER_SIZE];
		debug(buffer, BUFFER_SIZE, engine, msg);
		Serial.print(buffer);
	}
}