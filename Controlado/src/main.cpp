#include <Arduino.h>
#include "engine.hpp"
#include "controller.hpp"
#include "macro.hpp"

engine_t engine_left = ENGINE_FRONT_STOP;
engine_t engine_right = ENGINE_FRONT_STOP;
controller_t controller;

void setup()
{
	Serial.begin(115200);
	engine_begin();
	controller_begin("08:d1:f9:c8:f5:3c");
	macro_load();
	Serial.println("Ready.");
}

void loop()
{
	if (controller_is_connected())
	{
		engine_alive();
	}
	else
	{
		engine_kill();
		return;
	}

	macro_call();
	engine_left = ENGINE_FRONT_STOP;
	engine_right = ENGINE_FRONT_STOP;
	controller = controller_create_snapshot();

	if (controller.l2)
	{
		engine_left = {ENGINE_DIRECTION_BACK, controller.l2_value};
		engine_right = {ENGINE_DIRECTION_BACK, controller.l2_value};
		if (controller.l_stick_x <= -50)
		{
			engine_right.speed >>= 3;
		}
		else if (controller.l_stick_x > 50)
		{
			engine_left.speed >>= 3;
		}
	}
	else if (controller.r2)
	{
		engine_left = {ENGINE_DIRECTION_FRONT, controller.r2_value};
		engine_right = {ENGINE_DIRECTION_FRONT, controller.r2_value};
		if (controller.l_stick_x <= -50)
		{
			engine_right.speed >>= 3;
		}
		else if (controller.l_stick_x > 50)
		{
			engine_left.speed >>= 3;
		}
	}
	else if (controller.l_stick_x <= -50)
	{
		engine_left = {ENGINE_DIRECTION_BACK, 120};
		engine_right = {ENGINE_DIRECTION_FRONT, 120};
	}
	else if (controller.l_stick_x > 50)
	{
		engine_left = {ENGINE_DIRECTION_FRONT, 120};
		engine_right = {ENGINE_DIRECTION_BACK, 120};
	}

	Serial.printf("L2:%i R2:%i VelE:%f VelD:%f\n", controller.l2, controller.r2, engine_left.speed, engine_right.speed);
	engine_move(engine_left, engine_right);
}
