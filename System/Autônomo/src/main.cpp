#include <Arduino.h>
#include <engine.hpp>
#include <internal.hpp>
#include <receiver.hpp>
#include <sensor.hpp>
#include <serial.hpp>
#include <utilitie.hpp>
#include "main.hpp"

#pragma region "Main State Data"
uint8_t loop_state = LOOP_STATE_SETUP;
uint8_t sensor_usage = SENSOR_USAGE_NONE;
#pragma endregion "Main State Data"

#pragma region "Main Input Data"
char bluetooth_input_char = ESTRATEGIA_LOOP;
char estrategia = bluetooth_input_char;
#pragma endregion "Main Input Data"

#pragma region "Main Sensor Data"
direction_t direction_init = right;
direction_t direction_update = right;
tt::sensor_t sensor;
#pragma endregion "Main Sensor Data"

#pragma region "Main Setup"
void setup_task()
{
	TaskHandle_t SensorTask;
	xTaskCreatePinnedToCore(sensor_task, "SensorTask", STACK_SIZE, nullptr, 16, &SensorTask, PRO_CPU_NUM);
	vTaskDelay(512);
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
	bluetooth_input_char = ESTRATEGIA_LOOP;
	estrategia = bluetooth_input_char;

	while (bluetooth_input_char != COMMAND_SETUP)
	{
		vTaskDelay(1);
		if (tt::serial::available())
		{
			bluetooth_input_char = tt::serial::read();
		}
		else
		{
			continue;
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
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LADINHO, "LADINHO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_COSTAS, "COSTAS");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_DEFESA, "DEFESA");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_PRECISO, "PRECISO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_TRANQUILO, "TRANQUILO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LOOP, "É apenas um loop");
			break;

		case COMMAND_LEFT:
			direction_init = left;
			tt::serial::printf(STRLN("+---------+----------+"));
			tt::serial::printf(STRLN("| Direção | Esquerda |"));
			tt::serial::printf(STRLN("+---------+----------+"));
			break;

		case COMMAND_RIGHT:
			direction_init = right;
			tt::serial::printf(STRLN("+---------+---------+"));
			tt::serial::printf(STRLN("| Direção | Direita |"));
			tt::serial::printf(STRLN("+---------+---------+"));
			break;

		case COMMAND_CHECK:
			tt::serial::printf(STRLN("+------------+----------+"));
			tt::serial::printf(STRLN("| Estratégia | %c        |"), estrategia);
			tt::serial::printf(STRLN("| Direção    | %s |"), (direction_init == right ? "direita " : "esquerda"));
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

				case ESTRATEGIA_LADINHO:
					tt::serial::printf(STRLN("+------------+------------+"));
					tt::serial::printf(STRLN("| Estratégia | LADINHO!!! |"));
					tt::serial::printf(STRLN("+------------+------------+"));
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

				case ESTRATEGIA_PRECISO:
					tt::serial::printf(STRLN("+------------+------------+"));
					tt::serial::printf(STRLN("| Estratégia | PRECISO!!! |"));
					tt::serial::printf(STRLN("+------------+------------+"));
					break;

				case ESTRATEGIA_TRANQUILO:
					tt::serial::printf(STRLN("+------------+--------------+"));
					tt::serial::printf(STRLN("| Estratégia | TRANQUILO!!! |"));
					tt::serial::printf(STRLN("+------------+--------------+"));
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
				vTaskDelay(48);
				tt::internal::set_led(false);
				vTaskDelay(48);
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
				vTaskDelay(96);
				tt::internal::set_led(false);
				vTaskDelay(96);
			}
			break;
		case tt::receiver_t::end:
			for (int i = 0; i < 2; i++)
			{
				tt::internal::set_led(true);
				vTaskDelay(144);
				tt::internal::set_led(false);
				vTaskDelay(144);
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
	Serial.printf(STRLN("Mac Address: %s"), tt::internal::mac_address());
	vTaskDelay(512);

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
void __init__()
{
	tt::engine::init();

#if __DEBUG_ENGINE_STOP__
	tt::engine::set_standby(true);
#endif

	setup_estrategia();
	Serial.printf(STRLN("Setup Estrategia!"));

	setup_luta();
	Serial.printf(STRLN("Setup Luta!"));

	sensor_usage = SENSOR_USAGE_ALL;
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

	case ESTRATEGIA_LADINHO:
		tt::serial::printf(STRLN("CURVINHAA!!!"));
		inicio_ladinho();
		break;

	case ESTRATEGIA_COSTAS:
		tt::serial::printf(STRLN("COSTAS!!!"));
		inicio_costas();
		break;

	case ESTRATEGIA_DEFESA:
		tt::serial::printf(STRLN("DEFESA!!!"));
		inicio_defesa(DEFESA_TRIGGER, DEFESA_QUOTA);
		break;

	case ESTRATEGIA_PRECISO:
		tt::serial::printf(STRLN("PRECISO!!!"));
		inicio_preciso(PRECISO_TRIGGER, PRECISO_QUOTA);
		break;

	case ESTRATEGIA_TRANQUILO:
		tt::serial::printf(STRLN("TRANQUILO!!!"));
		inicio_tranquilo(TRANQUILO_TRIGGER, TRANQUILO_QUOTA);
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
		procura_padrao(TT_ENGINE_SPEED(ROTATE_SPEED));
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
		sensor_usage = SENSOR_USAGE_ALL;
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
		if (sensor.left && sensor_usage == SENSOR_USAGE_ALL)
		{
			direction_update = left;
		}
		else if (sensor.right && sensor_usage == SENSOR_USAGE_ALL)
		{
			direction_update = right;
		}

#if (__DEBUG_SHOW_SENSOR__)
		tt::serial::printf(STRLN("l:%i f:%i r:%i"), sensor.left, sensor.front, sensor.right);
#endif
#if (__DEBUG_SHOW_DIRECTION__)
		tt::serial::printf(STRLN("d:%i"), static_cast<int>(direction_update));
#endif
#if (__DEBUG_SHOW_ENGINE__)
		tt::serial::printf(STRLN("d:%i"), static_cast<int>(direction_update));
#endif
	}

	tt::internal::set_led(true);
	tt::engine::set_standby(true);
	tt::engine::stop();
	tt::serial::end();
	ESP.restart();
}
#pragma endregion "Main Task"

#pragma region "Main Inicio Estratégias"
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
	if (direction_init == right)
	{
		direction_update = left;
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_SLOW(2), TT_ENGINE_FRONT_FULL);
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		direction_update = right;
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_SLOW(2));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}

void inicio_curvinha()
{
	if (direction_init == right)
	{
		direction_update = left;
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_SLOW(2), TT_ENGINE_FRONT_FULL);
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		direction_update = right;
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_SLOW(2));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}

void inicio_ladinho()
{
	if (direction_init == right)
	{
		direction_update = left;
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_SLOW(3), TT_ENGINE_FRONT_FULL);
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		direction_update = right;
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_SLOW(3));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}

void inicio_costas()
{
	if (direction_init == right)
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

void inicio_defesa(int trigger, const int quota)
{
	tt::engine::move(TT_ENGINE_FRONT_SLOW(1), TT_ENGINE_FRONT_SLOW(1));
	vTaskDelay(16);
	tt::engine::stop();
	while (trigger < quota)
	{
		vTaskDelay(1);
		trigger += (sensor.left + sensor.front + sensor.right == 3);
		if (sensor.left && sensor.left + sensor.front + sensor.right <= 2)
		{
			tt::engine::move(TT_ENGINE_BACK(DEFESA_SPEED), TT_ENGINE_FRONT(DEFESA_SPEED));
			continue;
		}
		if (sensor.right && sensor.left + sensor.front + sensor.right <= 2)
		{
			tt::engine::move(TT_ENGINE_FRONT(DEFESA_SPEED), TT_ENGINE_BACK(DEFESA_SPEED));
			continue;
		}
		tt::engine::move(TT_ENGINE_FRONT_SLOW(4), TT_ENGINE_FRONT_SLOW(4));
	}
	vTaskDelay(24);
}

void inicio_preciso(int trigger, const int quota)
{
	tt::engine::move(TT_ENGINE_FRONT_SLOW(1), TT_ENGINE_FRONT_SLOW(1));
	vTaskDelay(16);
	tt::engine::stop();
	while (trigger < quota)
	{
		vTaskDelay(1);
		trigger += (sensor.left + sensor.front + sensor.right == 3);
		if (sensor.left && sensor.left + sensor.front + sensor.right <= 2)
		{
			tt::engine::move(TT_ENGINE_BACK(DEFESA_SPEED), TT_ENGINE_FRONT(DEFESA_SPEED));
			continue;
		}
		if (sensor.right && sensor.left + sensor.front + sensor.right <= 2)
		{
			tt::engine::move(TT_ENGINE_FRONT(DEFESA_SPEED), TT_ENGINE_BACK(DEFESA_SPEED));
			continue;
		}
		tt::engine::move(TT_ENGINE_FRONT_SLOW(2), TT_ENGINE_FRONT_SLOW(2));
	}
}

void inicio_tranquilo(int trigger, const int quota)
{
	tt::engine::move(TT_ENGINE_FRONT_SLOW(1), TT_ENGINE_FRONT_SLOW(1));
	vTaskDelay(16);
	tt::engine::stop();
	while (trigger < quota)
	{
		vTaskDelay(1);
		trigger += (sensor.left + sensor.front + sensor.right == 3);
		if (sensor.left && sensor.left + sensor.front + sensor.right <= 2)
		{
			tt::engine::move(TT_ENGINE_BACK(DEFESA_SPEED), TT_ENGINE_FRONT(DEFESA_SPEED));
			continue;
		}
		if (sensor.right && sensor.left + sensor.front + sensor.right <= 2)
		{
			tt::engine::move(TT_ENGINE_FRONT(DEFESA_SPEED), TT_ENGINE_BACK(DEFESA_SPEED));
			continue;
		}
		tt::engine::move(TT_ENGINE_FRONT_SLOW(1), TT_ENGINE_FRONT_SLOW(1));
	}
	inicio_curvinha();
}
#pragma endregion "Main Inicio Estratégias"

#pragma region "Main Procura Estratégias"
void procura_padrao(uint8_t velocidade_giro)
{
	if (sensor.front)
	{
		tt::engine::stop();
		for (uint8_t i = TT_ENGINE_SPEED_SLOW(2); i < TT_ENGINE_SPEED_FULL && sensor.front; i += 2)
		{
			uint8_t left_v = TT_ENGINE_SPEED(i);
			uint8_t right_v = TT_ENGINE_SPEED(i);
			if (sensor.left + sensor.front + sensor.right < 3)
			{
				if (sensor.left)
				{
					left_v >>= 6;
				}
				else if (sensor.right)
				{
					right_v >>= 6;
				}
			}
			tt::engine::move(TT_ENGINE_FRONT(left_v), TT_ENGINE_FRONT(right_v));
			vTaskDelay(1);
		}
		while (sensor.front)
		{
			uint8_t left_v = TT_ENGINE_SPEED_FULL;
			uint8_t right_v = TT_ENGINE_SPEED_FULL;
			if (sensor.left + sensor.front + sensor.right < 3)
			{
				if (sensor.left)
				{
					left_v >>= 6;
				}
				else if (sensor.right)
				{
					right_v >>= 6;
				}
			}
			tt::engine::move(TT_ENGINE_FRONT(left_v), TT_ENGINE_FRONT(right_v));
			vTaskDelay(1);
		}
	}
	if (sensor.left || direction_update == left)
	{
		tt::engine::move(TT_ENGINE_BACK(velocidade_giro), TT_ENGINE_FRONT(velocidade_giro));
	}
	else if (sensor.right || direction_update == right)
	{
		tt::engine::move(TT_ENGINE_FRONT(velocidade_giro), TT_ENGINE_BACK(velocidade_giro));
	}
}
#pragma endregion "Main Procura Estratégias"

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