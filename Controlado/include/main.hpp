#include <cstdint>
#include <cstddef>

#define LOOP_STATE_INIT 0
#define LOOP_STATE_UPDATE 1

#pragma region "Main Signatures"
void init();
void update();
void update_engine();
void reset_engine();
void macro_normal();
void macro_careful();
void macro_just_go();
void macro_forward(const uint8_t direction, const uint8_t speed_modifier);
void macro_curve(const uint8_t left_direction, const uint8_t righ_direction);
#pragma endregion "Main Signatures"