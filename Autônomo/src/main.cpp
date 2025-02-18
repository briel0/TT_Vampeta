#include <Arduino.h>
#include <engine.hpp>
#include <internal.hpp>
#include <receiver.hpp>
#include <sensor.hpp>
#include <serial.hpp>
#include <utilitie.hpp>
#include "main.hpp"

uint8_t loop_state = LOOP_STATE_SETUP;

char bluetooth_input_char = ESTRATEGIA_LOOP;
char estrategia = bluetooth_input_char;

direction_t direction = right;
tt::sensor_t sensor;

#pragma region "Main Setup"
void setup_task()
{
	TaskHandle_t SensorTask;
	xTaskCreatePinnedToCore(sensor_task, "SensorTask", 256 * 16, nullptr, 16, &SensorTask, PRO_CPU_NUM);
	vTaskDelay(500);
}

void setup_connect()
{
	while (!tt::serial::enabled())
	{
		vTaskDelay(1);
	}
	
	tt::serial::printf(STRLN("LIGOUUUU"));
	tt::serial::printf(STRLN("Digite '%c' para Mostrar os Comandos Disponíveis!"), COMMAND_HELP);
}

void setup_estrategia()
{
	while (bluetooth_input_char != COMMAND_SETUP)
	{
		vTaskDelay(1);
		if (tt::serial::available())
		{
			bluetooth_input_char = tt::serial::read();
		}

		switch (bluetooth_input_char)
		{
		case COMMAND_HELP:
			tt::serial::clear();
			tt::serial::printf(STRLN("+-----------------+"));
			tt::serial::printf(STRLN("| UM HELP PRA TU! |"));
			tt::serial::printf(STRLN("+-----------------+"));
			tt::serial::printf(STRLN("+-------------------------------+"));
			tt::serial::printf(STRLN("| Todos os Comandos (MAIÚSCULA) |"));
			tt::serial::printf(STRLN("+-------------------------------+"));
			tt::serial::printf(STRLN("\"Comando\" -> \"Nome\""));
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_HELP, "HELP");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_CHECK, "CHECK");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_LEFT, "LEFT");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_RIGHT, "RIGHT");
			tt::serial::printf(STRLN("+-----------------------------+"));
			tt::serial::printf(STRLN("| Todos os Testes (MAIÚSCULA) |"));
			tt::serial::printf(STRLN("+-----------------------------+"));
			tt::serial::printf(STRLN("\"Teste\" -> \"Nome\""));
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), TEST_SENSOR, "SENSOR");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), TEST_ENGINE, "ENGINE");
			tt::serial::printf(STRLN("+----------------------------------+"));
			tt::serial::printf(STRLN("| Todas as Estratégias (MINÚSCULA) |"));
			tt::serial::printf(STRLN("+----------------------------------+"));
			tt::serial::printf(STRLN("\"Estrategia\" -> \"Nome\""));
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTAO, "FRENTÃO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTINHA, "FRENTINHA");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVAO, "CURVÃO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVINHA, "CURVINHA");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_COSTAS, "COSTAS");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_DEFESA, "DEFESA");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LOOP, "É apenas um loop");
			break;

		case COMMAND_LEFT:
			direction = left;
			tt::serial::printf(STRLN("+---------+----------+"));
			tt::serial::printf(STRLN("| Direção | Esquerda |"));
			tt::serial::printf(STRLN("+---------+----------+"));
			break;

		case COMMAND_RIGHT:
			direction = right;
			tt::serial::printf(STRLN("+---------+---------+"));
			tt::serial::printf(STRLN("| Direção | Direita |"));
			tt::serial::printf(STRLN("+---------+---------+"));
			break;

		case COMMAND_CHECK:
			tt::serial::printf(STRLN("+------------+----------+"));
			tt::serial::printf(STRLN("| Estratégia | %c        |"), estrategia);
			tt::serial::printf(STRLN("| Direção    | %s |"), (direction == right ? "direita " : "esquerda"));
			tt::serial::printf(STRLN("+------------+----------+"));
			break;

		case COMMAND_SETUP:
			tt::serial::printf(STRLN("+------------------+"));
			tt::serial::printf(STRLN("| SETUP FINALIZADO |"));
			tt::serial::printf(STRLN("+------------------+"));
			continue;
			break;

		case TEST_SENSOR:
			test_sensor();
			break;

		case TEST_ENGINE:
			test_engine();
			break;

		default:
			if (CHARACTER_IS_LOWER_CASE(bluetooth_input_char))
			{
				estrategia = bluetooth_input_char;
				switch (bluetooth_input_char)
				{
				case ESTRATEGIA_FRENTAO:
					tt::serial::printf(STRLN("+------------+------------+"));
					tt::serial::printf(STRLN("| Estratégia | FRENTÃO!!! |"));
					tt::serial::printf(STRLN("+------------+------------+"));
					break;

				case ESTRATEGIA_FRENTINHA:
					tt::serial::printf(STRLN("+------------+--------------+"));
					tt::serial::printf(STRLN("| Estratégia | FRENTINHA!!! |"));
					tt::serial::printf(STRLN("+------------+--------------+"));
					break;

				case ESTRATEGIA_CURVAO:
					tt::serial::printf(STRLN("+------------+-----------+"));
					tt::serial::printf(STRLN("| Estratégia | CURVÃO!!! |"));
					tt::serial::printf(STRLN("+------------+-----------+"));
					break;

				case ESTRATEGIA_CURVINHA:
					tt::serial::printf(STRLN("+------------+-------------+"));
					tt::serial::printf(STRLN("| Estratégia | CURVINHA!!! |"));
					tt::serial::printf(STRLN("+------------+-------------+"));
					break;

				case ESTRATEGIA_COSTAS:
					tt::serial::printf(STRLN("+------------+-----------+"));
					tt::serial::printf(STRLN("| Estratégia | COSTAS!!! |"));
					tt::serial::printf(STRLN("+------------+-----------+"));
					break;

				case ESTRATEGIA_DEFESA:
					tt::serial::printf(STRLN("+------------+-----------+"));
					tt::serial::printf(STRLN("| Estratégia | DEFESA!!! |"));
					tt::serial::printf(STRLN("+------------+-----------+"));
					break;

				default:
					break;
				}
			}
			break;
		}
	}
}

void setup_luta()
{
	bool ready = false;
	bool update = false;
	do
	{
		vTaskDelay(1);
		update = tt::receiver::update();
		if (!update)
		{
			continue;
		}
		switch (tt::receiver::receiver())
		{
		case tt::receiver_t::test:
			ready = true;
			for (int i = 0; i < 3; i++)
			{
				tt::internal::set_led(true);
				vTaskDelay(50);
				tt::internal::set_led(false);
				vTaskDelay(50);
			}
			break;
		case tt::receiver_t::begin:
			if (ready)
			{
				break;
			}
			for (int i = 0; i < 2; i++)
			{
				tt::internal::set_led(true);
				vTaskDelay(100);
				tt::internal::set_led(false);
				vTaskDelay(100);
			}
			break;
		case tt::receiver_t::end:
			for (int i = 0; i < 2; i++)
			{
				tt::internal::set_led(true);
				vTaskDelay(150);
				tt::internal::set_led(false);
				vTaskDelay(150);
			}
			break;
		}
	} while (!(ready && tt::receiver::receiver() == tt::receiver_t::begin));

	tt::internal::set_led(false);
	tt::engine::set_standby(false);
}

void setup()
{
	Serial.begin(115200);
	Serial.printf(STRLN("Serial 115200!"));

	tt::internal::setup();
	tt::internal::set_led(false);
	Serial.printf(STRLN("Setup Internal!"));

	tt::engine::setup();
	Serial.printf(STRLN("Setup Engine!"));

	tt::sensor::setup();
	Serial.printf(STRLN("Setup Sensor!"));

	tt::receiver::setup();
	Serial.printf(STRLN("Setup Receiver!"));

	tt::serial::setup(ROBO_NAME);
	Serial.printf(STRLN("Setup Serial!"));

	setup_task();
	Serial.printf(STRLN("Setup Task!"));

	setup_connect();
	Serial.printf(STRLN("Setup Connect!"));

	loop_state = LOOP_STATE_INIT;
}
#pragma endregion "Main Setup"

#pragma region "Main Loop"
int cc = 0;
void __init__()
{
	tt::engine::init();
#if DEBUG_ENGINE_STOP
	tt::engine::set_standby(true);
#endif

	setup_estrategia();
	Serial.printf(STRLN("Setup Estrategia!"));

	setup_luta();
	Serial.printf(STRLN("Setup Luta!"));

	tt::serial::printf(STRLN("Começou!"));

	switch (estrategia)
	{
	case ESTRATEGIA_FRENTAO:
		tt::serial::printf(STRLN("FRENTAO!!!"));
		inicio_frentao();
		break;

	case ESTRATEGIA_FRENTINHA:
		tt::serial::printf(STRLN("FRENTINHA!!!"));
		inicio_frentinha();
		break;

	case ESTRATEGIA_CURVAO:
		tt::serial::printf(STRLN("CURVÃOO!!!"));
		inicio_curvao();
		break;

	case ESTRATEGIA_CURVINHA:
		tt::serial::printf(STRLN("CURVINHAA!!!"));
		inicio_curvinha();
		break;

	case ESTRATEGIA_COSTAS:
		tt::serial::printf(STRLN("COSTAS!!!"));
		inicio_costas();
		break;

	case ESTRATEGIA_DEFESA:
		tt::serial::printf(STRLN("DEFESA!!!"));
		inicio_defesa();
		break;

	case ESTRATEGIA_LOOP:
		tt::serial::printf(STRLN("LOOP!!!"));
		break;

	default:
		tt::serial::printf(STRLN("default"));
		inicio_frentao();
		break;
	}
}

void __update__()
{
	switch (estrategia)
	{
	default:
		procurar_padrao(TT_ENGINE_SPEED(ROTATE_SPEED));
		break;
	}
}

void loop()
{
	switch (loop_state)
	{
	case LOOP_STATE_INIT:
		__init__();
		loop_state = LOOP_STATE_UPDATE;
		break;
	case LOOP_STATE_UPDATE:
		__update__();
		break;
	default:
		return;
		break;
	}
}
#pragma endregion "Main Loop"

#pragma region "Main Task"
void sensor_task(void *pvParameters)
{
	while (!tt::receiver::signal(tt::receiver_t::end))
	{
		vTaskDelay(1);
		sensor = tt::sensor::create_snapshot();
		if (sensor.left)
		{
			direction = left;
		}
		else if (sensor.right)
		{
			direction = right;
		}

#if (DEBUG_SHOW_SENSOR)
		tt::serial::printf(STRLN("l:%i f:%i r:%i"), sensor.left, sensor.front, sensor.right);
#endif
#if (DEBUG_SHOW_DIRECTION)
		tt::serial::printf(STRLN("d:%i"), static_cast<int>(direction));
#endif
#if (DEBUG_SHOW_ENGINE)
		tt::serial::printf(STRLN("d:%i"), static_cast<int>(direction));
#endif
	}

	tt::internal::set_led(true);
	tt::engine::set_standby(true);
	tt::engine::stop();
	tt::serial::end();
	ESP.restart();
}
#pragma endregion "Main Task"

#pragma region "Main Estratégias"
void inicio_frentao()
{
	tt::engine::move(TT_ENGINE_FRONT(48), TT_ENGINE_FRONT(48));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(60), TT_ENGINE_FRONT(60));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(72), TT_ENGINE_FRONT(72));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(84), TT_ENGINE_FRONT(84));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(96), TT_ENGINE_FRONT(96));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(128), TT_ENGINE_FRONT(128));
	vTaskDelay(16);

	tt::engine::move(TT_ENGINE_FRONT(160), TT_ENGINE_FRONT(160));
	vTaskDelay(16);

	tt::engine::move(TT_ENGINE_FRONT(196), TT_ENGINE_FRONT(196));
	vTaskDelay(24);

	tt::engine::move(TT_ENGINE_FRONT(228), TT_ENGINE_FRONT(228));
	vTaskDelay(24);

	tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(255));
	vTaskDelay(48);
}

void inicio_frentinha()
{
	tt::engine::move(TT_ENGINE_FRONT(48), TT_ENGINE_FRONT(48));
	vTaskDelay(4);

	tt::engine::move(TT_ENGINE_FRONT(60), TT_ENGINE_FRONT(60));
	vTaskDelay(4);

	tt::engine::move(TT_ENGINE_FRONT(72), TT_ENGINE_FRONT(72));
	vTaskDelay(4);

	tt::engine::move(TT_ENGINE_FRONT(84), TT_ENGINE_FRONT(84));
	vTaskDelay(4);

	tt::engine::move(TT_ENGINE_FRONT(96), TT_ENGINE_FRONT(96));
	vTaskDelay(4);

	tt::engine::move(TT_ENGINE_FRONT(128), TT_ENGINE_FRONT(128));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(160), TT_ENGINE_FRONT(160));
	vTaskDelay(8);

	tt::engine::move(TT_ENGINE_FRONT(196), TT_ENGINE_FRONT(196));
	vTaskDelay(12);

	tt::engine::move(TT_ENGINE_FRONT(228), TT_ENGINE_FRONT(228));
	vTaskDelay(12);

	tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(255));
	vTaskDelay(48);
}

void inicio_curvao()
{
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_SLOW(2), TT_ENGINE_FRONT_FULL);
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		direction = left;
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_SLOW(2));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		direction = right;
	}
}

void inicio_curvinha()
{
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_SLOW(2), TT_ENGINE_FRONT_FULL);
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		direction = left;
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_SLOW(2));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		direction = right;
	}
}

void inicio_costas()
{
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(104);
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(104);
	}
}

void inicio_defesa()
{
	tt::engine::move(TT_ENGINE_FRONT(ROTATE_SPEED), TT_ENGINE_FRONT(ROTATE_SPEED));
	vTaskDelay(16);
	tt::engine::stop();
	while (sensor.left + sensor.front + sensor.right <= 2)
	{
		vTaskDelay(1);
		if (sensor.left)
		{
			tt::engine::move(TT_ENGINE_BACK(DEFESA_SPEED), TT_ENGINE_FRONT(DEFESA_SPEED));
			continue;
		}
		if (sensor.right)
		{
			tt::engine::move(TT_ENGINE_FRONT(DEFESA_SPEED), TT_ENGINE_BACK(DEFESA_SPEED));
			continue;
		}
		tt::engine::move(TT_ENGINE_FRONT_SLOW(4), TT_ENGINE_FRONT_SLOW(4));
	}
	vTaskDelay(64);
}

void procurar_padrao(uint8_t velocidade_giro)
{
	if (sensor.front)
	{
		tt::engine::stop();
		tt::engine::move(TT_ENGINE_FRONT_SLOW(1), TT_ENGINE_FRONT_SLOW(1));
		vTaskDelay(16);
		for (uint8_t i = TT_ENGINE_SPEED_SLOW(2); i < TT_ENGINE_SPEED_FULL && sensor.front; i += 6)
		{
			uint8_t left_v = TT_ENGINE_SPEED(i);
			uint8_t right_v = TT_ENGINE_SPEED(i);
			if (sensor.left + sensor.front + sensor.right < 3) {
				if (sensor.left) {
					left_v >>= 4;
				}
				else if (sensor.right) {
					right_v >>= 4;
				}
			}
			tt::engine::move(TT_ENGINE_FRONT(left_v), TT_ENGINE_FRONT(right_v));
			vTaskDelay(4);
		}
		while (sensor.front)
		{
			uint8_t left_v = TT_ENGINE_SPEED_FULL;
			uint8_t right_v = TT_ENGINE_SPEED_FULL;
			if (sensor.left + sensor.front + sensor.right < 3) {
				if (sensor.left) {
					left_v >>= 4;
				}
				else if (sensor.right) {
					right_v >>= 4;
				}
			}
			tt::engine::move(TT_ENGINE_FRONT(left_v), TT_ENGINE_FRONT(right_v));
			vTaskDelay(1);
		}
	}
	if (sensor.left || direction == left)
	{
		tt::engine::move(TT_ENGINE_BACK(velocidade_giro), TT_ENGINE_FRONT(velocidade_giro));
	}
	else if (sensor.right || direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT(velocidade_giro), TT_ENGINE_BACK(velocidade_giro));
	}
}
#pragma endregion "Main Estratégias"

#pragma region "Main Testes"
void test_sensor()
{
	for (int count = 0; count < TESTE_REPEAT; count++)
	{
		char buff[BUFFER_SIZE];
		tt::sensor::debug(buff, BUFFER_SIZE, sensor, "sensor");
		tt::serial::printf(buff);
		vTaskDelay(32);
	}
	tt::serial::clear();
	tt::serial::printf(STRLN("END TEST SENSOR!"));
}

void test_engine()
{
	tt::serial::printf(STRLN("(LEFT: FRONT) (RIGHT: FRONT)"));
	tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_FULL);
	vTaskDelay(3072);
	tt::engine::stop();
	vTaskDelay(512);
	tt::serial::printf(STRLN("(LEFT: FRONT) (RIGHT: BACK)"));
	tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
	vTaskDelay(3072);
	tt::engine::stop();
	vTaskDelay(512);
	tt::serial::printf(STRLN("(LEFT: BACK) (RIGHT: FRONT)"));
	tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
	vTaskDelay(3072);
	tt::engine::stop();
	vTaskDelay(512);
	tt::serial::printf(STRLN("(LEFT: BACK) (RIGHT: BACK)"));
	tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_BACK_FULL);
	vTaskDelay(3072);
	tt::engine::stop();
	vTaskDelay(512);
	tt::serial::printf(STRLN("(LEFT: FAST FRONT) (RIGHT: SLOW FRONT)"));
	tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_SLOW(4));
	vTaskDelay(3072);
	tt::engine::stop();
	vTaskDelay(512);
	tt::serial::printf(STRLN("(LEFT: SLOW FRONT) (RIGHT:  FAST FRONT)"));
	tt::engine::move(TT_ENGINE_FRONT_SLOW(4), TT_ENGINE_FRONT_FULL);
	vTaskDelay(3072);
	tt::engine::stop();
	tt::serial::printf(STRLN("END TEST ENGINE!"));
}
#pragma endregion "Main Testes"