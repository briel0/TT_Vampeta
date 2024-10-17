#include <Arduino.h>
#include "controller.hpp"
#include "engine.hpp"
#include "internal.hpp"

#define LOOP_STATE_INIT 0
#define LOOP_STATE_UPDATE 1

uint8_t loop_state = LOOP_STATE_INIT;
engine_t engine_left = ENGINE_DEFAULT;
engine_t engine_right = ENGINE_DEFAULT;
controller_t controller;

#pragma region "Main Signatures"
void init();
void update();
void update_engine();
void reset_engine();
void macro_normal();
void macro_careful();
void macro_just_go();
void macro_forward(const uint8_t direction, const uint8_t speed_modifier);
void macro_curve(const uint8_t left_direction, const uint8_t righ_direction);
#pragma endregion "Main Signatures"

#pragma region "Main Setup"
void setup()
{
	Serial.begin(115200);
	Serial.println("Serial 115200!");

	controller_begin("F0:24:F9:44:F8:DE");
	engine_begin();
	internal_begin();
	Serial.println("Begin and Load Complete!");

	engine_standby(false);
	internal_led(false);
	Serial.println("Setup Complete!");
}
#pragma endregion "Main Setup"

#pragma region "Main Loop"
void init()
{
}

void update()
{
	internal_led(true);
	if (controller_disconnected())
	{
		internal_led(false);
		goto loop_reset_engine;
	}

	macro_normal();
	controller = controller_create_snapshot();

	if (controller.triangle)
	{
		Serial.println("(controller.triangle)");
		macro_careful();
	}

	if (controller.cross)
	{
		Serial.println("(controller.cross)");
		macro_just_go();
		goto loop_update_engine;
	}

	if (controller.l2)
	{
		Serial.println("(controller.l2)");
		macro_forward(ENGINE_DIRECTION_BACK, controller.l2_value);
		goto loop_update_engine;
	}

	if (controller.r2)
	{
		Serial.println("(controller.r2)");
		macro_forward(ENGINE_DIRECTION_FRONT, controller.r2_value);
		goto loop_update_engine;
	}

	if (controller.l_stick_x <= -50)
	{
		Serial.println("(controller.l_stick_x <= -50)");
		macro_curve(ENGINE_DIRECTION_BACK, ENGINE_DIRECTION_FRONT);
		internal_setup_millis();
		goto loop_update_engine;
	}

	if (controller.l_stick_x >= 50)
	{
		Serial.println("(controller.l_stick_x >= 50)");
		macro_curve(ENGINE_DIRECTION_FRONT, ENGINE_DIRECTION_BACK);
		internal_setup_millis();
		goto loop_update_engine;
	}

loop_reset_engine:
	reset_engine();

loop_update_engine:
	update_engine();
}

void loop()
{
	switch (loop_state)
	{
	case LOOP_STATE_INIT:
		init();
		loop_state = LOOP_STATE_UPDATE;
		break;
	case LOOP_STATE_UPDATE:
		update();
		break;
	default:
		break;
	}
}
#pragma endregion "Main Loop"

#pragma region "Main Functions"
void update_engine()
{
	Serial.printf("################################################################################################################################\n");
	engine_debug(engine_left, "engine_left");
	engine_debug(engine_right, "engine_right");
	controller_debug(controller, "controller");
	internal_debug("internal");
	engine_move(engine_left, engine_right);
}

void reset_engine()
{
	engine_stop();
	engine_left = ENGINE_FRONT_STOP;
	engine_right = ENGINE_FRONT_STOP;
	internal_setup_millis();
}

void macro_normal()
{
	const uint8_t base_speed = static_cast<uint8_t>(INTERNAL_BETWEEN(internal_delta_millis() / 3, ENGINE_SPEED_SLOW(2), ENGINE_SPEED_FULL));
	engine_left.speed = base_speed;
	engine_right.speed = base_speed;
}

void macro_careful()
{
	const uint8_t base_speed = static_cast<uint8_t>(INTERNAL_BETWEEN(internal_delta_millis() / 2, ENGINE_SPEED_SLOW(2), ENGINE_SPEED_FULL));
	const uint8_t slow_base_speed = map(base_speed, ENGINE_SPEED_SLOW(2), ENGINE_SPEED_FULL, ENGINE_SPEED_SLOW(3), ENGINE_SPEED_SLOW(2));
	engine_left.speed = slow_base_speed;
	engine_right.speed = slow_base_speed;
}

void macro_just_go()
{
	engine_left = ENGINE_FRONT_FULL;
	engine_right = ENGINE_FRONT_FULL;
}

void macro_forward(const uint8_t direction, const uint8_t speed_modifier)
{
	const uint8_t left_speed = map(speed_modifier, ENGINE_SPEED_STOP, ENGINE_SPEED_FULL,
								   ENGINE_SPEED_STOP, static_cast<uint8_t>(engine_left.speed));
	const uint8_t right_speed = map(speed_modifier, ENGINE_SPEED_STOP, ENGINE_SPEED_FULL,
									ENGINE_SPEED_STOP, static_cast<uint8_t>(engine_right.speed));
	engine_left = {direction, left_speed};
	engine_right = {direction, right_speed};
	if (controller.l_stick_x <= -50)
	{
		engine_right.speed >>= 3;
	}
	else if (controller.l_stick_x >= 50)
	{
		engine_left.speed >>= 3;
	}
}

void macro_curve(const uint8_t left_direction, const uint8_t righ_direction)
{
	engine_left.direction = left_direction;
	engine_right.direction = righ_direction;
}
#pragma endregion "Main Functions"