struct controller_t
{
	bool triangle, square, circle, cross;
	bool r2, l2;
	uint8_t r2_value, l2_value;
	int8_t r_stick_x, r_stick_y;
	int8_t l_stick_x, l_stick_y;
};

void controller_begin(const char *mac);
bool controller_is_connected();
controller_t controller_get();