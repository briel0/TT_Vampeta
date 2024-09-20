#define pwmA 23
#define a1 16
#define a2 17
#define pwmB 4
#define b1 18
#define b2 19
#define stby 5
#define led 2
#define IR 15

#define FRONT 1
#define BACK 2

#define FRONT_FULL {FRONT, UINT8_MAX}
#define BACK_FULL {BACK, UINT8_MAX}

#define FRONT_STOP {FRONT, 0}
#define BACK_STOP {BACK, 0}

struct engine_t
{
	uint8_t direction;
	uint8_t speed;
};

void engine_begin();
void engine_move(engine_t engine_left, engine_t engine_right);
void engine_stop();