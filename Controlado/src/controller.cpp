#include <Arduino.h>
#include "controller.hpp"

#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

void controller_begin(const char *mac)
{
	PS4.begin(mac);
}

bool controller_is_connected()
{
	return PS4.isConnected();
}

controller_t controller_get()
{
	controller_t controller;

	controller.triangle = PS4.Triangle();
	controller.square = PS4.Square();
	controller.circle = PS4.Circle();
	controller.cross = PS4.Cross();

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