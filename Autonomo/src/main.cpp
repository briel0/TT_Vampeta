#include <Arduino.h>
#include <engine.hpp>
#include <internal.hpp>
#include <receiver.hpp>
#include <sensor.hpp>
#include <serial.hpp>
#include "main.hpp"

uint8_t loop_state = LOOP_STATE_INIT;

char bluetooth_input_char = ESTRATEGIA_LOOP;
char estrategia = bluetooth_input_char;
bool sensor_running = false;

direction_t direction = right;
tt::sensor_t sensor;

#pragma region "Main Setup"
void setup_task()
{
	xTaskCreatePinnedToCore(sensor_task, "SensorTask", 2048, nullptr, 16, nullptr, PRO_CPU_NUM);
	vTaskDelay(500);
}

void setup_estrategia()
{
	tt::serial::println("LIGOUUUU");
	tt::serial::printf("Digite '%c' para Mostrar os Comandos Disponíveis!!\n", COMMAND_HELP);

	while (bluetooth_input_char != COMMAND_SETUP)
	{
		if (tt::serial::available())
		{
			bluetooth_input_char = tt::serial::read();
			tt::serial::println(bluetooth_input_char);
		}

		switch (bluetooth_input_char)
		{
		case COMMAND_HELP:
			tt::serial::printf(STRLN("UM HELP PRA TU!"));
			tt::serial::printf(STRLN("Todos os Comandos (CARACTER MAIUSCULA):"));
			tt::serial::printf(STRLN("\"Catacter do Comando\" -> \"Nome do Comando\""));
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_HELP, "HELP");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_CHECK, "CHECK");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_LEFT, "LEFT");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), COMMAND_RIGHT, "RIGHT");
			tt::serial::printf(STRLN("Todas as Estratégias: (CARACTER MINUSCULA):"));
			tt::serial::printf(STRLN("\"Caracter da Estrategia\" -> \"Nome da Estratégia\""));
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTAO, "FRENTÃO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTINHA, "FRENTINHA");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVAO, "CURVÃO");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVINHA, "CURVINHA");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_COSTAS, "COSTAS");
			tt::serial::printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LOOP, "Sem estratégia, apenas um loop");
			break;

		case COMMAND_LEFT:
			direction = left;
			break;

		case COMMAND_RIGHT:
			direction = right;
			break;

		case COMMAND_CHECK:
			tt::serial::println("Check");
			tt::serial::print("Estrategia: ");
			tt::serial::println(estrategia);
			tt::serial::print("Direc: ");
			if (direction == right)
			{
				tt::serial::println("direita");
			}
			else
			{
				tt::serial::println("esquerda");
			}
			break;

		case COMMAND_SETUP:
			continue;
			break;

		default:
			if (CHARACTER_IS_LOWER_CASE(bluetooth_input_char))
			{
				estrategia = bluetooth_input_char;
			}
			break;
		}
	}
	//tt::serial::end();
}

void setup_luta()
{
	bool ready = false;
	while (true)
	{
		if (tt::receiver::decode())
		{
			tt::receiver::resume();
			if (tt::receiver::command() == 0x0)
			{
				ready = true;
				for (int i = 0; i < 3; i++)
				{
					tt::internal::set_led(true);
					vTaskDelay(50);
					tt::internal::set_led(false);
					vTaskDelay(50);
					if (tt::receiver::decode())
					{
						tt::receiver::resume();
						if (tt::receiver::command() == 0x1 && ready)
						{
							tt::internal::set_led(false);
							tt::engine::set_standby(false);
							break;
						}
					}
				}
			}

			if (tt::receiver::command() == 0x1 && ready)
			{
				tt::internal::set_led(false);
				tt::engine::set_standby(false);
				break;
			}
		}
	}
	sensor_running = true;
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Serial 115200!");

	tt::engine::setup();
	tt::internal::setup();
	tt::sensor::setup();
	Serial.println("Begin and Load Complete!");

	tt::engine::set_standby(false);
	tt::internal::set_led(false);
	Serial.println("Setup Complete!");

	tt::engine::set_standby(true);
	tt::receiver::setup();
	tt::serial::setup(ROBO_NAME);

	setup_task();
	setup_estrategia();
	setup_luta();
	tt::serial::println("COMEÇOOUU!!!");
}
#pragma endregion "Main Setup"

#pragma region "Main Loop"
void init()
{
	tt::engine::init();

	switch (estrategia)
	{
	case ESTRATEGIA_FRENTAO:
		tt::serial::println("FRENTAO!!!");
		inicio_frentao();
		break;

	case ESTRATEGIA_FRENTINHA:
		tt::serial::println("FRENTINHA!!!");
		inicio_frentinha();
		break;

	case ESTRATEGIA_CURVAO:
		tt::serial::println("CURVÃOO!!!");
		inicio_curvao();
		break;

	case ESTRATEGIA_CURVINHA:
		tt::serial::println("CURVINHAA!!!");
		inicio_curvinha();
		break;

	case ESTRATEGIA_COSTAS:
		tt::serial::println("COSTAS!!!");
		inicio_costas();
		break;

	case ESTRATEGIA_LOOP:
		tt::serial::println("LOOP!!!");
		break;

	default:
		tt::serial::println("default");
		inicio_frentao();
		break;
	}
}

void update()
{
	switch (estrategia)
	{
	default:
		procurar_padrao(TT_ENGINE_SPEED(88));
		break;
	}
}

void loop()
{
	switch (loop_state)
	{
	case LOOP_STATE_INIT:
		init();
		loop_state = LOOP_STATE_UPDATE;
		break;
	case LOOP_STATE_UPDATE:
		update();
		break;
	default:
		break;
	}
}
#pragma endregion "Main Loop"

#pragma region "Main Task"
void sensor_task(void *pvParameters)
{
	while (true)
	{
		if (sensor_running)
		{
			if (tt::receiver::decode())
			{
				tt::receiver::resume();
				if (tt::receiver::command() == 0x2)
				{
					tt::internal::set_led(true);
					tt::engine::stop();
					ESP.restart();
				}
			}

			sensor = tt::sensor::create_snapshot();
			if (sensor.left)
			{
				direction = left;
			}
			else if (sensor.right)
			{
				direction = right;
			}
		}
		vTaskDelay(1);
	}
}
#pragma endregion "Main Task"

#pragma region "Main Estratégias"
void inicio_frentao()
{
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
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(64));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		direction = left;
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(64), TT_ENGINE_FRONT(255));
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
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(64));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		direction = left;
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(64), TT_ENGINE_FRONT(255));
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

void procurar_padrao(uint8_t velocidade_giro)
{
	if (sensor.front)
	{
		tt::engine::stop();
		tt::engine::move(TT_ENGINE_FRONT_SLOW(1), TT_ENGINE_FRONT_SLOW(1));
		vTaskDelay(16);
		for (uint8_t i = TT_ENGINE_SPEED_SLOW(1); i < TT_ENGINE_SPEED_FULL && sensor.front; i += 16)
		{
			tt::engine::move(TT_ENGINE_FRONT(i), TT_ENGINE_FRONT(i));
			vTaskDelay(8);
		}
		while (sensor.front)
		{
			tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_FRONT_FULL);
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
