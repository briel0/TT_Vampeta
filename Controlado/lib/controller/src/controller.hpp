#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace controller
	{
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

		void begin(const char *mac);
		bool is_connected();
		bool disconnected();
		controller_t create_snapshot();
		void debug(char *out_buffer, const size_t out_size, const controller_t controller, const char *msg);
		void debug(const controller_t controller, const char *msg);
	}

	typedef controller::controller_t controller_t;
}