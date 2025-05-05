#include <cstdint>
#include <cstddef>

#pragma region "Main Data Defines"
#define ROBO_NAME "Arruela"
#define BUFFER_SIZE 512
#pragma endregion "Main Data Defines"

#pragma region "Main Command Defines"
#define COMMAND_HELP 'H'
#define COMMAND_SETUP '.'
#define TEST_SENSOR 'S'
#define TEST_ENGINE 'E'
#define DEBUG_ENGINE 'a'
#define DEBUG_RECEIVER 'b'
#define DEBUG_CLOSE '-'
#pragma endregion "Main Command Defines"

#pragma region "Main Debug Defines"
#pragma endregion "Main Debug Defines"

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
void debug_engine(const tt::engine_t engine_left, const tt::engine_t engine_right, const int timer);
void debug_receiver();
void test_sensor();
void test_engine();
void sensor_task(void *pvParameters);
void setup_task();
void setup_connect();
void setup_debug();
void __init__();
void __update__();
#pragma endregion "Main Signatures"

#pragma region "Main Templates Signatures"
template <typename T>
void setup_data(const char *msg, T &data);
#pragma endregion "Main Templates Signatures"