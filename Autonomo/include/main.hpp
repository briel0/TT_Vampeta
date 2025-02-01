#include <cstdint>
#include <cstddef>

#define ROBO_NAME "Arruela"
#define STRLN(x) x "\n"

#define CHARACTER_IS_LOWER_CASE(x) (x >= 'a' && x <= 'z')
#define CHARACTER_IS_UPPER_CASE(x) (x >= 'A' && x <= 'Z')
#define CHARACTER_IS_NUMBER(x) (x >= '0' && x <= '9')
#define COMMAND_HELP 'H'
#define COMMAND_CHECK 'C'
#define COMMAND_SETUP '0'
#define COMMAND_LEFT '<'
#define COMMAND_RIGHT '>'
#define ESTRATEGIA_FRENTAO 'a'
#define ESTRATEGIA_FRENTINHA 'b'
#define ESTRATEGIA_CURVAO 'c'
#define ESTRATEGIA_CURVINHA 'd'
#define ESTRATEGIA_COSTAS 'e'
#define ESTRATEGIA_DEFESA 'f'
#define ESTRATEGIA_LOOP 'z'

#define ROTATE_SPEED 88

#define LOOP_STATE_INIT 0
#define LOOP_STATE_UPDATE 1

#define DEBUG_SHOW_SENSOR false
#define DEBUG_SHOW_DIRECTION false
#define DEBUG_SHOW_ENGINE false
#define DEBUG_ENGINE_STOP false

enum direction_t
{
	left,
	right,
};

#pragma region "Main Signatures"
void inicio_frentao();
void inicio_frentinha();
void inicio_curvao();
void inicio_curvinha();
void inicio_costas();
void inicio_defesa();
void procurar_padrao(uint8_t velocidade_giro);
void sensor_task(void *pvParameters);
void setup_task();
void setup_estrategia();
void setup_luta();
void init();
void update();
#pragma endregion "Main Signatures"