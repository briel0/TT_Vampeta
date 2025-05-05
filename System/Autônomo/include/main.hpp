#include <cstdint>
#include <cstddef>

#pragma region "Main Data Defines"
#define ROBO_NAME "Arruela"
#define BUFFER_SIZE 512
#pragma endregion "Main Data Defines"

#pragma region "Main Command Defines"
#define COMMAND_HELP 'H'
#define COMMAND_CHECK 'C'
#define COMMAND_SETUP '.'
#define COMMAND_LEFT '<'
#define COMMAND_RIGHT '>'
#define TEST_SENSOR 'S'
#define TEST_ENGINE 'E'
#define ESTRATEGIA_FRENTAO 'a'
#define ESTRATEGIA_FRENTINHA 'b'
#define ESTRATEGIA_CURVAO 'c'
#define ESTRATEGIA_CURVINHA 'd'
#define ESTRATEGIA_LADINHO 'e'
#define ESTRATEGIA_COSTAS 'f'
#define ESTRATEGIA_DEFESA 'g'
#define ESTRATEGIA_PRECISO 'h'
#define ESTRATEGIA_TRANQUILO 'i'
#define ESTRATEGIA_LOOP 'z'
#pragma endregion "Main Command Defines"

#pragma region "Main Estratégia Defines"
#define ROTATE_SPEED 96
#define DEFESA_SPEED (ROTATE_SPEED - 8)
#define DEFESA_TRIGGER 0
#define DEFESA_QUOTA 256
#define PRECISO_TRIGGER 0
#define PRECISO_QUOTA 96
#define TRANQUILO_TRIGGER 0
#define TRANQUILO_QUOTA 192
#pragma endregion "Main Estratégia Defines"

#pragma region "Main Test Defines"
#define TESTE_REPEAT 512
#pragma endregion "Main Test Defines"

#pragma region "Main Loop Defines"
#define LOOP_STATE_SETUP 0
#define LOOP_STATE_INIT 1
#define LOOP_STATE_UPDATE 2
#pragma endregion "Main Loop Defines"

#pragma region "Main Sensor Defines"
#define SENSOR_USAGE_NONE 0
#define SENSOR_USAGE_ALL 1
#pragma endregion "Main Sensor Defines"

#pragma region "Main Debug Defines"
#define __DEBUG_SHOW_SENSOR__ false
#define __DEBUG_SHOW_DIRECTION__ false
#define __DEBUG_SHOW_ENGINE__ false
#define __DEBUG_ENGINE_STOP__ false
#pragma endregion "Main Debug Defines"

#pragma region "Main Macros"
#define CHARACTER_IS_LOWER_CASE(x) (x >= 'a' && x <= 'z')
#define CHARACTER_IS_UPPER_CASE(x) (x >= 'A' && x <= 'Z')
#define CHARACTER_IS_NUMBER(x) (x >= '0' && x <= '9')
#define STRLN(x) x "\n"
#pragma endregion "Main Macros"

#pragma region "Main Types"
enum direction_t
{
	left,
	right,
};
#pragma endregion "Main Types"

#pragma region "Main Signatures"
void inicio_frentao();
void inicio_frentinha();
void inicio_curvao();
void inicio_curvinha();
void inicio_ladinho();
void inicio_costas();
void inicio_defesa(int trigger, const int quota);
void inicio_preciso(int trigger, const int quota);
void inicio_tranquilo(int trigger, const int quota);
void procura_padrao(uint8_t velocidade_giro);
void test_sensor();
void test_engine();
void sensor_task(void *pvParameters);
void setup_task();
void setup_connect();
void setup_estrategia();
void setup_luta();
void __init__();
void __update__();
#pragma endregion "Main Signatures"