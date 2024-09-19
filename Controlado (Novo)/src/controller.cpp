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