#include <stdint.h>

struct controller_t
{
	bool right, down, up, left;
	bool upright, downright, upleft, downleft;
	bool square, cross, circle, triangle;
	bool l1, r1, l2, r2;
	uint8_t l2_value, r2_value;
	int8_t l_stick_x, l_stick_y;
	int8_t r_stick_x, r_stick_y;
};

void controller_begin(const char *mac);
bool controller_is_connected();
bool controller_disconnected();
controller_t controller_create_snapshot();
void controller_debug(const controller_t controller, const char *msg);