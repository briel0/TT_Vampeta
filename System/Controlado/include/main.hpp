#include <cstdint>
#include <cstddef>

#define LOOP_STATE_INIT 0
#define LOOP_STATE_UPDATE 1
#define STICK_TRIGGER 48

#define RC_STATE_CAREFUL 0
#define RC_STATE_NORMAL 1

enum direction_t
{
	left,
	right,
};

#pragma region "Main Signatures"
void init();
void update();
void update_engine();
void reset_engine();
void modifier_normal();
void modifier_careful();
void behavior_just_go();
void behavior_forward(const uint8_t direction, const uint8_t speed_modifier);
void behavior_curve(const uint8_t left_direction, const uint8_t righ_direction);
void macro_curvinha(const direction_t left_direction);
void macro_curvao(const direction_t left_direction);
#pragma endregion "Main Signatures"