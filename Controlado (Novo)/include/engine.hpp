#define pwmA 23
#define a1 16
#define a2 17
#define pwmB 4
#define b1 18
#define b2 19
#define stby 5
#define led 2
#define IR 15

#define FRENTE 1
#define TRAS 2

void engine_begin();
void engine_move(byte sentido_esquerda, byte velocidade_esquerda, byte sentido_direita, byte velocidade_direita);
void engine_stop();