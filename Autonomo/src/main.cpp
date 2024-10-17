#include <Arduino.h>
#include "engine.hpp"
#include "internal.hpp"
#include "receiver.hpp"
#include "sensor.hpp"
#include "serial.hpp"

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
#define ESTRATEGIA_LOOP 'z'

#define LOOP_STATE_INIT 0
#define LOOP_STATE_UPDATE 1

enum direction_t
{
	left,
	right,
};

uint8_t loop_state = LOOP_STATE_INIT;

char bluetooth_input_char = ESTRATEGIA_LOOP;
char estrategia = bluetooth_input_char;
bool sensor_running = false;

direction_t direction = right;
sensor_t sensor;

#pragma region "Main Signatures"
void inicio_frentao();
void inicio_frentinha();
void inicio_curvao();
void inicio_curvinha();
void inicio_costas();
void procurar_padrao(uint8_t velocidade_giro);
void sensor_task(void *pvParameters);
void setup_task();
void setup_estrategia();
void setup_luta();
void loop_init();
void loop_update();
#pragma endregion "Main Signatures"

#pragma region "Main Estratégias"
void inicio_frentao()
{
	engine_move(ENGINE_FRONT(72), ENGINE_FRONT(72));
	vTaskDelay(8);

	engine_move(ENGINE_FRONT(84), ENGINE_FRONT(84));
	vTaskDelay(8);

	engine_move(ENGINE_FRONT(96), ENGINE_FRONT(96));
	vTaskDelay(8);

	engine_move(ENGINE_FRONT(128), ENGINE_FRONT(128));
	vTaskDelay(16);

	engine_move(ENGINE_FRONT(160), ENGINE_FRONT(160));
	vTaskDelay(16);

	engine_move(ENGINE_FRONT(196), ENGINE_FRONT(196));
	vTaskDelay(24);

	engine_move(ENGINE_FRONT(228), ENGINE_FRONT(228));
	vTaskDelay(24);

	engine_move(ENGINE_FRONT(255), ENGINE_FRONT(255));
	vTaskDelay(48);
}

void inicio_frentinha()
{
	engine_move(ENGINE_FRONT(72), ENGINE_FRONT(72));
	vTaskDelay(4);

	engine_move(ENGINE_FRONT(84), ENGINE_FRONT(84));
	vTaskDelay(4);

	engine_move(ENGINE_FRONT(96), ENGINE_FRONT(96));
	vTaskDelay(4);

	engine_move(ENGINE_FRONT(128), ENGINE_FRONT(128));
	vTaskDelay(8);

	engine_move(ENGINE_FRONT(160), ENGINE_FRONT(160));
	vTaskDelay(8);

	engine_move(ENGINE_FRONT(196), ENGINE_FRONT(196));
	vTaskDelay(12);

	engine_move(ENGINE_FRONT(228), ENGINE_FRONT(228));
	vTaskDelay(12);

	engine_move(ENGINE_FRONT(255), ENGINE_FRONT(255));
	vTaskDelay(48);
}

void inicio_curvao()
{
	if (direction == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		vTaskDelay(70);
		engine_move(ENGINE_FRONT(255), ENGINE_FRONT(64));
		vTaskDelay(392);
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		vTaskDelay(70);
		direction = left;
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		vTaskDelay(70);
		engine_move(ENGINE_FRONT(64), ENGINE_FRONT(255));
		vTaskDelay(392);
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		vTaskDelay(70);
		direction = right;
	}
}

void inicio_curvinha()
{
	if (direction == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		vTaskDelay(70);
		engine_move(ENGINE_FRONT(255), ENGINE_FRONT(64));
		vTaskDelay(196);
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		vTaskDelay(70);
		direction = left;
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		vTaskDelay(70);
		engine_move(ENGINE_FRONT(64), ENGINE_FRONT(255));
		vTaskDelay(196);
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		vTaskDelay(70);
		direction = right;
	}
}

void inicio_costas()
{
	if (direction == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		vTaskDelay(104);
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		vTaskDelay(104);
	}
}

void procurar_padrao(uint8_t velocidade_giro)
{
	if (sensor.front)
	{
		engine_stop();
		engine_move(ENGINE_FRONT_SLOW(1), ENGINE_FRONT_SLOW(1));
		vTaskDelay(16);
		for (uint8_t i = ENGINE_SPEED_SLOW(1); i < ENGINE_SPEED_FULL && sensor.front; i += 16)
		{
			engine_move(ENGINE_FRONT(i), ENGINE_FRONT(i));
			vTaskDelay(8);
		}
		while (sensor.front)
		{
			engine_move(ENGINE_FRONT_FULL, ENGINE_FRONT_FULL);
		}
	}
	if (sensor.left || direction == left)
	{
		engine_move(ENGINE_BACK(velocidade_giro), ENGINE_FRONT(velocidade_giro));
	}
	else if (sensor.right || direction == right)
	{
		engine_move(ENGINE_FRONT(velocidade_giro), ENGINE_BACK(velocidade_giro));
	}
}
#pragma endregion "Main Estratégias"

#pragma region "Main Task"
void sensor_task(void *pvParameters)
{
	while (true)
	{
		if (sensor_running)
		{
			if (receiver_decode())
			{
				receiver_resume();
				if (receiver_command() == 0x2)
				{
					internal_led(true);
					engine_stop();
					ESP.restart();
				}
			}

			sensor = sensor_create_snapshot();
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

#pragma region "Main Setup"
void setup_task()
{
	xTaskCreatePinnedToCore(sensor_task, "SensorTask", 2048, nullptr, 16, nullptr, PRO_CPU_NUM);
	vTaskDelay(500);
}

void setup_estrategia()
{
	serial_println("LIGOUUUU");
	serial_printf("Digite '%c' para Mostrar os Comandos Disponíveis!!\n", COMMAND_HELP);

	while (bluetooth_input_char != COMMAND_SETUP)
	{
		if (serial_available())
		{
			bluetooth_input_char = serial_read();
			serial_println(bluetooth_input_char);
		}

		switch (bluetooth_input_char)
		{
		case COMMAND_HELP:
			serial_printf(STRLN("UM HELP PRA TU!"));
			serial_printf(STRLN("Todos os Comandos (CARACTER MAIUSCULA):"));
			serial_printf(STRLN("\"Catacter do Comando\" -> \"Nome do Comando\""));
			serial_printf(STRLN("\"%c\" -> \"%s\""), COMMAND_HELP, "HELP");
			serial_printf(STRLN("\"%c\" -> \"%s\""), COMMAND_CHECK, "CHECK");
			serial_printf(STRLN("\"%c\" -> \"%s\""), COMMAND_LEFT, "LEFT");
			serial_printf(STRLN("\"%c\" -> \"%s\""), COMMAND_RIGHT, "RIGHT");
			serial_printf(STRLN("Todas as Estratégias: (CARACTER MINUSCULA):"));
			serial_printf(STRLN("\"Caracter da Estrategia\" -> \"Nome da Estratégia\""));
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTAO, "FRENTÃO");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTINHA, "FRENTINHA");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVAO, "CURVÃO");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVINHA, "CURVINHA");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_COSTAS, "COSTAS");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LOOP, "Sem estratégia, apenas um loop");
			break;

		case COMMAND_LEFT:
			direction = left;
			break;

		case COMMAND_RIGHT:
			direction = right;
			break;

		case COMMAND_CHECK:
			serial_println("Check");
			serial_print("Estrategia: ");
			serial_println(estrategia);
			serial_print("Direc: ");
			if (direction == right)
			{
				serial_println("direita");
			}
			else
			{
				serial_println("esquerda");
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
	serial_end();
}

void setup_luta()
{
	bool ready = false;
	while (true)
	{
		if (receiver_decode())
		{
			receiver_resume();
			if (receiver_command() == 0x0)
			{
				ready = true;
				for (int i = 0; i < 3; i++)
				{
					internal_led(true);
					vTaskDelay(50);
					internal_led(false);
					vTaskDelay(50);
					if (receiver_decode())
					{
						receiver_resume();
						if (receiver_command() == 0x1 && ready)
						{
							internal_led(false);
							engine_standby(false);
							break;
						}
					}
				}
			}

			if (receiver_command() == 0x1 && ready)
			{
				internal_led(false);
				engine_standby(false);
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

	engine_begin();
	internal_begin();
	sensor_begin();
	Serial.println("Begin and Load Complete!");

	engine_standby(false);
	internal_led(false);
	Serial.println("Setup Complete!");

	engine_standby(true);
	receiver_begin();
	serial_begin(ROBO_NAME);

	setup_task();
	setup_estrategia();
	setup_luta();
	serial_println("COMEÇOOUU!!!");
}
#pragma endregion "Main Setup"

#pragma region "Main Loop"
void loop_init()
{
	// setup_luta();
	// setup_task();

	switch (estrategia)
	{
	case ESTRATEGIA_FRENTAO:
		serial_println("FRENTAO!!!");
		inicio_frentao();
		break;

	case ESTRATEGIA_FRENTINHA:
		serial_println("FRENTINHA!!!");
		inicio_frentinha();
		break;

	case ESTRATEGIA_CURVAO:
		serial_println("CURVÃOO!!!");
		inicio_curvao();
		break;

	case ESTRATEGIA_CURVINHA:
		serial_println("CURVINHAA!!!");
		inicio_curvinha();
		break;

	case ESTRATEGIA_COSTAS:
		serial_println("COSTAS!!!");
		inicio_costas();
		break;

	case ESTRATEGIA_LOOP:
		serial_println("LOOP!!!");
		break;

	default:
		serial_println("default");
		inicio_frentao();
		break;
	}
}

void loop_update()
{
	switch (estrategia)
	{
	default:
		procurar_padrao(ENGINE_SPEED(88));
		break;
	}
}

void loop()
{
	switch (loop_state)
	{
	case LOOP_STATE_INIT:
		loop_init();
		loop_state = LOOP_STATE_UPDATE;
		break;
	case LOOP_STATE_UPDATE:
		loop_update();
		break;
	default:
		break;
	}
}
#pragma endregion "Main Loop"