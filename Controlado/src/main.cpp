#include <Arduino.h>
#include "engine.hpp"
#include "controller.hpp"

engine_t engine_left = FRONT_STOP;
engine_t engine_right = FRONT_STOP;
controller_t controller;

void setup()
{
	Serial.begin(115200);
	engine_begin();
	controller_begin("08:d1:f9:c8:f5:3c");
	Serial.println("Ready.");
}

void loop()
{
	digitalWrite(2, HIGH);
	if (!controller_is_connected())
	{
		digitalWrite(2, LOW);
		engine_stop();
		return;
	}

	engine_left = FRONT_STOP;
	engine_right = FRONT_STOP;
	controller = controller_get();

	while (controller.cross)
	{
		engine_move(FRONT_FULL, FRONT_FULL);
		controller = controller_get();
	}

	if (controller.r2)
	{
		engine_left = {FRONT, controller.r2_value};
		engine_right = {FRONT, controller.r2_value};
		if (controller.l_stick_x <= -50)
		{
			engine_right.speed >>= 3;
		}
		else if (controller.l_stick_x > 50)
		{
			engine_left.speed >>= 3;
		}
	}
	else if (controller.l2)
	{
		engine_left = {BACK, controller.l2_value};
		engine_right = {BACK, controller.l2_value};
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
		engine_left = {BACK, 120};
		engine_right = {FRONT, 120};
	}
	else if (controller.l_stick_x > 50)
	{
		engine_left = {FRONT, 120};
		engine_right = {BACK, 120};
	}

	Serial.printf("L2:%i R2:%i VelE:%f VelD:%f\n", controller.l2, controller.r2, engine_left.speed, engine_right.speed);
	engine_move(engine_left, engine_right);
}
