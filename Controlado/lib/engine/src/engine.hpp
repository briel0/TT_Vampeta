#include <cstdint>
#include <cstddef>

#pragma region "Engine Direction Macros"
#define TT_ENGINE_DIRECTION_DEFAULT 0
#define TT_ENGINE_DIRECTION_FRONT 0
#define TT_ENGINE_DIRECTION_BACK 1
#pragma endregion "Engine Direction Macros"

#pragma region "Engine Speed Macros"
#define TT_ENGINE_SPEED_DEFAULT static_cast<uint8_t>(0)
#define TT_ENGINE_SPEED_FULL static_cast<uint8_t>(UINT8_MAX)
#define TT_ENGINE_SPEED_SLOW(x) static_cast<uint8_t>(TT_ENGINE_SPEED_FULL >> x)
#define TT_ENGINE_SPEED_STOP static_cast<uint8_t>(0)
#define TT_ENGINE_SPEED(x) static_cast<uint8_t>((x >= TT_ENGINE_SPEED_STOP && x <= TT_ENGINE_SPEED_FULL) * (x) + (TT_ENGINE_SPEED_FULL) * (x > TT_ENGINE_SPEED_FULL))
#pragma endregion "Engine Speed Macros"

#pragma region "Engine Macros"
#define TT_ENGINE_DEFAULT {TT_ENGINE_DIRECTION_DEFAULT, TT_ENGINE_SPEED_DEFAULT}
#define TT_ENGINE_FRONT_FULL {TT_ENGINE_DIRECTION_FRONT, TT_ENGINE_SPEED_FULL}
#define TT_ENGINE_BACK_FULL {TT_ENGINE_DIRECTION_BACK, TT_ENGINE_SPEED_FULL}
#define TT_ENGINE_FRONT_SLOW(x) {TT_ENGINE_DIRECTION_FRONT, TT_ENGINE_SPEED_SLOW(x)}
#define TT_ENGINE_BACK_SLOW(x) {TT_ENGINE_DIRECTION_BACK, TT_ENGINE_SPEED_SLOW(x)}
#define TT_ENGINE_FRONT_STOP {TT_ENGINE_DIRECTION_FRONT, TT_ENGINE_SPEED_STOP}
#define TT_ENGINE_BACK_STOP {TT_ENGINE_DIRECTION_BACK, TT_ENGINE_SPEED_STOP}
#define TT_ENGINE_FRONT(x) {TT_ENGINE_DIRECTION_FRONT, TT_ENGINE_SPEED(x)}
#define TT_ENGINE_BACK(x) {TT_ENGINE_DIRECTION_BACK, TT_ENGINE_SPEED(x)}
#pragma endregion "Engine Macros"

namespace tt
{
	namespace engine
	{
		struct engine_t
		{
			uint8_t direction;
			uint8_t speed;

			bool operator == (const engine_t &engine) const
			{
				return (direction == engine.direction && speed == engine.speed);
			}
			bool operator != (const engine_t &engine) const
			{
				return (direction != engine.direction || speed != engine.speed);
			}
		};

		void setup();
		bool get_standby();
		void set_standby(const bool mode);
		void init();
		void move(const engine_t engine_left, const engine_t engine_right);
		void stop();
		void debug(char *out_buffer, const size_t out_size, engine_t engine, const char *msg);
		void debug(engine_t engine, const char *msg);
	}

	typedef engine::engine_t engine_t;
}