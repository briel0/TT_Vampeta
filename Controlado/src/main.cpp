#include <Arduino.h>
#include <controller.hpp>
#include <engine.hpp>
#include <internal.hpp>
#include "main.hpp"

uint8_t loop_state = LOOP_STATE_INIT;
tt::engine_t engine_left = TT_ENGINE_DEFAULT;
tt::engine_t engine_right = TT_ENGINE_DEFAULT;
tt::controller_t controller;

#pragma region "Main Setup"
void setup()
{
	Serial.begin(115200);
	Serial.println("Serial 115200!");

	tt::controller::setup("f0:24:f9:44:2a:5c");
	tt::engine::setup();
	tt::internal::setup();
	Serial.println("Begin and Load Complete!");

	tt::engine::set_standby(false);
	tt::internal::set_led(false);
	Serial.println("Setup Complete!");
}
#pragma endregion "Main Setup"

#pragma region "Main Loop"
void init()
{
	tt::engine::init();
}

void update()
{
	tt::internal::set_led(true);
	if (tt::controller::disconnected())
	{
		tt::internal::set_led(false);
		goto loop_reset_engine;
	}

	modifier_normal();
	controller = tt::controller::create_snapshot();

	if (controller.triangle)
	{
		Serial.println("(controller.triangle)");
		modifier_careful();
	}

	if (controller.l1)
	{
		Serial.println("(controller.l1)");
		macro_curvinha(left);
		controller = tt::controller::create_snapshot();
	}

	if (controller.r1)
	{
		Serial.println("(controller.r1)");
		macro_curvinha(right);
		controller = tt::controller::create_snapshot();
	}

	if (controller.left)
	{
		Serial.println("(controller.left)");
		macro_curvao(left);
		controller = tt::controller::create_snapshot();
	}

	if (controller.right)
	{
		Serial.println("(controller.right)");
		macro_curvao(right);
		controller = tt::controller::create_snapshot();
	}

	if (controller.cross)
	{
		Serial.println("(controller.cross)");
		behavior_just_go();
		goto loop_update_engine;
	}

	if (controller.l2)
	{
		Serial.println("(controller.l2)");
		behavior_forward(TT_ENGINE_DIRECTION_BACK, controller.l2_value);
		goto loop_update_engine;
	}

	if (controller.r2)
	{
		Serial.println("(controller.r2)");
		behavior_forward(TT_ENGINE_DIRECTION_FRONT, controller.r2_value);
		goto loop_update_engine;
	}

	if (controller.l_stick_x <= -50)
	{
		Serial.println("(controller.l_stick_x <= -50)");
		behavior_curve(TT_ENGINE_DIRECTION_BACK, TT_ENGINE_DIRECTION_FRONT);
		tt::internal::setup_millis();
		goto loop_update_engine;
	}

	if (controller.l_stick_x >= 50)
	{
		Serial.println("(controller.l_stick_x >= 50)");
		behavior_curve(TT_ENGINE_DIRECTION_FRONT, TT_ENGINE_DIRECTION_BACK);
		tt::internal::setup_millis();
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
	tt::engine::debug(engine_left, "engine_left");
	tt::engine::debug(engine_right, "engine_right");
	tt::controller::debug(controller, "controller");
	tt::internal::debug("internal");
	tt::engine::move(engine_left, engine_right);
}

void reset_engine()
{
	tt::engine::stop();
	engine_left = TT_ENGINE_FRONT_STOP;
	engine_right = TT_ENGINE_FRONT_STOP;
	tt::internal::setup_millis();
}
#pragma endregion "Main Functions"

#pragma region "Modifier Functions"
void modifier_normal()
{
	const uint8_t base_speed = static_cast<uint8_t>(TT_INTERNAL_BETWEEN(tt::internal::delta_millis() / 2 + 32, TT_ENGINE_SPEED_SLOW(2), TT_ENGINE_SPEED_FULL));
	engine_left.speed = base_speed;
	engine_right.speed = base_speed;
}

void modifier_careful()
{
	const uint8_t base_speed = static_cast<uint8_t>(TT_INTERNAL_BETWEEN(tt::internal::delta_millis() / 2, TT_ENGINE_SPEED_SLOW(2), TT_ENGINE_SPEED_FULL));
	const uint8_t slow_base_speed = map(base_speed, TT_ENGINE_SPEED_SLOW(2), TT_ENGINE_SPEED_FULL, TT_ENGINE_SPEED_SLOW(3), TT_ENGINE_SPEED_SLOW(2));
	engine_left.speed = slow_base_speed;
	engine_right.speed = slow_base_speed;
}
#pragma endregion "Modifier Functions"

#pragma region "Behavior Functions"
void behavior_forward(const uint8_t direction, const uint8_t speed_modifier)
{
	const uint8_t left_speed = map(speed_modifier, TT_ENGINE_SPEED_STOP, TT_ENGINE_SPEED_FULL,
								   TT_ENGINE_SPEED_STOP, static_cast<uint8_t>(engine_left.speed));
	const uint8_t right_speed = map(speed_modifier, TT_ENGINE_SPEED_STOP, TT_ENGINE_SPEED_FULL,
									TT_ENGINE_SPEED_STOP, static_cast<uint8_t>(engine_right.speed));
	engine_left = {direction, left_speed};
	engine_right = {direction, right_speed};
	if (controller.l_stick_x <= -50)
	{
		engine_left.speed >>= 3;
	}
	else if (controller.l_stick_x >= 50)
	{
		engine_right.speed >>= 3;
	}
}

void behavior_curve(const uint8_t left_direction, const uint8_t righ_direction)
{
	engine_left.direction = left_direction;
	engine_right.direction = righ_direction;
}

void behavior_just_go()
{
	engine_left = TT_ENGINE_FRONT_FULL;
	engine_right = TT_ENGINE_FRONT_FULL;
}
#pragma endregion "Behavior Functions"

#pragma region "Macro Functions"
void macro_curvinha(const direction_t direction) {
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(64), TT_ENGINE_FRONT(255));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(64));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}

void macro_curvao(const direction_t direction) {
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(64), TT_ENGINE_FRONT(255));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(64));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}
#pragma endregion "Macro Functions"
