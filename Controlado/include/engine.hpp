#include <stdint.h>

#pragma region "Engine Direction Macros"
#define ENGINE_DIRECTION_FRONT 0
#define ENGINE_DIRECTION_BACK 1
#pragma endregion "Engine Direction Macros"

#pragma region "Engine Speed Macros"
#define ENGINE_SPEED_FULL UINT8_MAX
#define ENGINE_SPEED_SLOW(x) (ENGINE_SPEED_FULL >> x)
#define ENGINE_SPEED_STOP 0
#define ENGINE_SPEED(x) ((x >= ENGINE_SPEED_STOP && x <= ENGINE_SPEED_FULL) * (x) + (ENGINE_SPEED_FULL) * (x > ENGINE_SPEED_FULL))
#pragma endregion "Engine Speed Macros"

#pragma region "Engine Macros"
#define ENGINE_FRONT_FULL {ENGINE_DIRECTION_FRONT, ENGINE_SPEED_FULL}
#define ENGINE_BACK_FULL {ENGINE_DIRECTION_BACK, ENGINE_SPEED_FULL}
#define ENGINE_FRONT_STOP {ENGINE_DIRECTION_FRONT, ENGINE_SPEED_STOP}
#define ENGINE_BACK_STOP {ENGINE_DIRECTION_BACK, ENGINE_SPEED_STOP}
#define ENGINE_FRONT(x) {ENGINE_DIRECTION_FRONT, ENGINE_SPEED(x)}
#define ENGINE_BACK(x) {ENGINE_DIRECTION_BACK, ENGINE_SPEED(x)}
#pragma endregion "Engine Macros"

struct engine_t
{
	uint8_t direction;
	uint8_t speed;
};

void engine_begin();
void engine_standby(const bool mode);
void engine_move(const engine_t engine_left, const engine_t engine_right);
void engine_stop();
void engine_debug(const engine_t engine, const char *msg);