#include <cstdint>
#include <cstddef>

#define ROBO_NAME "Vampeta"

#ifndef BYTE_SIZE
#define BYTE_SIZE 256
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE (BYTE_SIZE * 2)
#endif

#ifndef STACK_SIZE
#define STACK_SIZE (BYTE_SIZE * 16)
#endif

#define LOOP_STATE_INIT 0
#define LOOP_STATE_UPDATE 1
#define STICK_TRIGGER 48

#define RC_STATE_CAREFUL 0
#define RC_STATE_NORMAL 1

#define STRLN(x) x "\n"

enum direction_t{
	left,
	right,
};

void __init__();
void __update__();
void update_engine();
void reset_engine();
void modifier_normal();
void modifier_careful();
void behavior_just_go();
void behavior_forward(const uint8_t direction, const uint8_t speed_modifier);
void behavior_curve(const uint8_t left_direction, const uint8_t righ_direction);
void macro_curvinha(const direction_t left_direction);
void macro_curvao(const direction_t left_direction);
void macro_ladinho(const direction_t left_direction);