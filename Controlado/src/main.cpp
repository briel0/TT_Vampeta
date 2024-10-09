#include <Arduino.h>
#include "controller.hpp"
#include "engine.hpp"
#include "internal.hpp"

float base_left_speed = 128, base_right_speed = 128;
engine_t engine_left = ENGINE_FRONT_STOP;
engine_t engine_right = ENGINE_FRONT_STOP;
controller_t controller;

#pragma region "Arrela Main Signatures"
void update_engine();
void macro_careful();
void macro_just_go();
void macro_forward(const uint8_t direction, const uint8_t speed_modifier);
void macro_curve(const uint8_t left_direction, const uint8_t righ_direction);
#pragma endregion "Arrela Main Signatures"

#pragma region "Arrela Main Setup"
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
#pragma endregion "Arrela Main Setup"

#pragma region "Arrela Main Loop"
void loop()
{
	internal_led(false);
	if (controller_disconnected())
	{
		internal_led(true);
		engine_stop();
		return;
	}

	engine_left = ENGINE_FRONT_STOP;
	engine_right = ENGINE_FRONT_STOP;
	controller = controller_create_snapshot();

	base_left_speed = 128 + map(static_cast<float>(internal_delta_millis()) / static_cast<float>(1000), 0, 127, 0, 127);
	base_left_speed = 128 + map(static_cast<float>(internal_delta_millis()) / static_cast<float>(1000), 0, 127, 0, 127);
	engine_left.speed = base_left_speed;
	engine_right.speed = base_right_speed;

	if (controller.triangle)
	{
		macro_careful();
	}

	if (controller.cross)
	{
		macro_just_go();
		goto loop_update_engine;
	}

	if (controller.l2)
	{
		macro_forward(ENGINE_DIRECTION_BACK, controller.l2_value);
		goto loop_update_engine;
	}

	if (controller.r2)
	{
		macro_forward(ENGINE_DIRECTION_FRONT, controller.r2_value);
		goto loop_update_engine;
	}

	if (controller.l_stick_x <= -50)
	{
		macro_curve(ENGINE_DIRECTION_BACK, ENGINE_DIRECTION_FRONT);
		goto loop_update_engine;
	}

	if (controller.l_stick_x >= 50)
	{
		macro_curve(ENGINE_DIRECTION_FRONT, ENGINE_DIRECTION_BACK);
		goto loop_update_engine;
	}

	internal_setup_millis();

loop_update_engine:
	update_engine();
}
#pragma endregion "Arrela Main Loop"

#pragma region "Arrela Main Functions"
void update_engine()
{
	Serial.printf("################################################################################################################################\n");
	engine_debug(engine_left, "engine_left");
	engine_debug(engine_right, "engine_right");
	controller_debug(controller, "controller");
	engine_move(engine_left, engine_right);
}

void macro_careful()
{
	engine_left.speed = ENGINE_SPEED(64);
	engine_right.speed = ENGINE_SPEED(64);
}

void macro_just_go()
{
	engine_left = ENGINE_FRONT_FULL;
	engine_right = ENGINE_FRONT_FULL;
}

void macro_forward(const uint8_t direction, const uint8_t speed_modifier)
{
	engine_left = {direction, speed_modifier};
	engine_right = {direction, speed_modifier};
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
	engine_left.speed >>= 1;
	engine_right.speed >>= 1;
	engine_left.direction = left_direction;
	engine_right.direction = righ_direction;
}
#pragma endregion "Arrela Main Functions"