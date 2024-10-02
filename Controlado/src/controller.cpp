#include <Arduino.h>
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>
#include "controller.hpp"

void controller_begin(const char *mac)
{
	PS4.begin(mac);
}

bool controller_is_connected()
{
	return PS4.isConnected();
}

controller_t controller_create_snapshot()
{
	controller_t controller;

	controller.right = PS4.Right();
	controller.down = PS4.Down();
	controller.up = PS4.Up();
	controller.left = PS4.Left();

	controller.upright = PS4.UpRight();
	controller.downright = PS4.DownRight();
	controller.upleft = PS4.UpLeft();
	controller.downleft = PS4.DownLeft();

	controller.square = PS4.Square();
	controller.cross = PS4.Cross();
	controller.circle = PS4.Circle();
	controller.triangle = PS4.Triangle();

	controller.r2 = PS4.R2();
	controller.l2 = PS4.L2();

	controller.r2_value = PS4.R2Value();
	controller.l2_value = PS4.L2Value();

	controller.r_stick_x = PS4.RStickX();
	controller.r_stick_y = PS4.RStickY();
	controller.l_stick_x = PS4.LStickX();
	controller.l_stick_y = PS4.LStickY();

	return controller;
}