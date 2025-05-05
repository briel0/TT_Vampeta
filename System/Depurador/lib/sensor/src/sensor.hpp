#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace sensor
	{
		struct sensor_t
		{
			int left;
			int front;
			int right;
		};

		void setup();
		sensor_t create_snapshot();
		void debug(char *out_buffer, const size_t out_size, sensor_t sensor, const char *msg);
		void debug(sensor_t sensor, const char *msg);
	}

	typedef sensor::sensor_t sensor_t;
}