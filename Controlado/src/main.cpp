#include <Arduino.h>
#include "controller.hpp"
#include "engine.hpp"
#include "internal.hpp"
#include "macro.hpp"

engine_t engine_left = ENGINE_FRONT_STOP;
engine_t engine_right = ENGINE_FRONT_STOP;
controller_t controller;

void setup()
{
	Serial.begin(115200);
	Serial.println("Serial 115200!");

	controller_begin("08:d1:f9:c8:f5:3c");
	engine_begin();
	internal_begin();
	macro_load();
	Serial.println("Begin and Load Complete!");

	engine_standby(false);
	internal_led(false);
	Serial.println("Setup Complete!");
}

void loop()
{
	if (controller_is_connected())
	{
		internal_led(false);
	}
	else
	{
		internal_led(true);
		engine_stop();
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
