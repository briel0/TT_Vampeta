#include "controller.hpp"

void controller_begin(const char *mac)
{
	PS4.begin(mac);
}