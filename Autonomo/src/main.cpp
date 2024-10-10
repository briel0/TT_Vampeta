#include <Arduino.h>
#include "engine.hpp"
#include "internal.hpp"
#include "receiver.hpp"
#include "sensor.hpp"
#include "serial.hpp"

#define ROBO_NAME "Arruela"
#define STRLN(x) x "\n"

#define COMMAND_HELP 'H'
#define ESTRATEGIA_FRENTAO 'a'
#define ESTRATEGIA_CURVAO 'b'
#define ESTRATEGIA_CURVINHA 'c'
#define ESTRATEGIA_COSTAS 'd'
#define ESTRATEGIA_LOOP 'z'

enum direction
{
	left,
	right,
};

TaskHandle_t SensorTask;

char BT = 'f';		   // char que é recebida pelo bluetooth
char estrategia = 'f'; // estratégio de início da luta (qualquer caracter)
bool sensor_running = false;

direction direc = left;

sensor_t sensor;
engine_t engine_left = ENGINE_FRONT_STOP;
engine_t engine_right = ENGINE_FRONT_STOP;

void inicio_frentao()
{
	engine_move(ENGINE_FRONT_FULL, ENGINE_FRONT_FULL);
	delay(325);
}

void inicio_curvao(direction init_direc)
{
	if (init_direc == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(70);
		engine_move(ENGINE_FRONT(200), ENGINE_FRONT(80));
		delay(300);
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(70);
		init_direc = left;
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(70);
		engine_move(ENGINE_FRONT(80), ENGINE_FRONT(200));
		delay(300);
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(70);
		init_direc = right;
	}
}

void inicio_curvinha(direction init_direc)
{
	if (init_direc == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(70);
		engine_move(ENGINE_FRONT(200), ENGINE_FRONT(80));
		delay(150);
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(70);
		init_direc = left;
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(70);
		engine_move(ENGINE_FRONT(80), ENGINE_FRONT(200));
		delay(150);
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(70);
		init_direc = right;
	}
}

void inicio_costas(direction init_direc)
{
	if (init_direc == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(190);
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(190);
	}
}

void procurar_padrao(uint8_t velocidade_giro)
{
	if (sensor.front)
	{
		while (sensor.front)
		{
			engine_move(ENGINE_FRONT_FULL, ENGINE_FRONT_FULL);
		}
	}
	else if (sensor.left)
	{
		engine_move(ENGINE_BACK(velocidade_giro), ENGINE_FRONT(velocidade_giro));
	}
	else if (sensor.right)
	{
		engine_move(ENGINE_FRONT(velocidade_giro), ENGINE_BACK(velocidade_giro));
	}
	else
	{
		engine_move(ENGINE_BACK(velocidade_giro), ENGINE_FRONT(velocidade_giro));
		// if (direc == esq)
		//	left(velocidade_giro, velocidade_giro);
		// else
		//	right(velocidade_giro, velocidade_giro);
	}
}

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

			sensor_t sensor = sensor_create_snapshot();
			if (sensor.left)
			{
				direc = left;
			}
			else if (sensor.right)
			{
				direc = right;
			}
		}
		vTaskDelay(1);
	}
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

	// #######################################################

	engine_standby(true);
	serial_begin(ROBO_NAME);

	serial_println("LIGOUUUU");
	serial_printf("Digite '%c' para Mostrar os Comandos Disponíveis!!\n", COMMAND_HELP);

	xTaskCreatePinnedToCore(sensor_task, "SensorTask", 10000, NULL, 1, &SensorTask, 0);
	delay(500);

	while (BT != '0')
	{ // Inicio quando bluetooth recebe o char '0'
		if (serial_available())
		{
			BT = serial_read();
			serial_println(BT);
		}

		if (BT == 'H')
		{
			serial_printf(STRLN("UM HELP PRA TU!"));
			serial_printf(STRLN("Todos os Comandos (CARACTER MAIUSCULA):"));
			serial_printf(STRLN("\"Catacter do Comando\" -> \"Nome do Comando\""));
			serial_printf(STRLN("\"%c\" -> \"%s\""), COMMAND_HELP, "HELP");
			serial_printf(STRLN("Todas as Estratégias: (CARACTER MINUSCULA):"));
			serial_printf(STRLN("\"Caracter da Estrategia\" -> \"Nome da Estratégia\""));
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTAO, "FRENTÃO");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVAO, "CURVÃO");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVINHA, "CURVINHA");
			// serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_DEFESA, "DEFESA");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_COSTAS, "COSTAS");
			// serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_DEFESA_FAKE, "DEFESA FAKE");
			// serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_ESTRATEGI, "ESTRATEGI");
			serial_printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LOOP, "Sem estratégia, apenas um loop");
		}
		else if (BT == '<')
		{ // definir para que lado será toda a progrmação
			direc = left;
		}
		else if (BT == '>')
		{
			direc = right;
		}
		else if (BT == 'C')
		{ // Char 'C' checa as estrategias
			serial_println("Check");
			serial_print("Estrategia: ");
			serial_println(estrategia);
			serial_print("Direc: ");
			if (direc == right)
				serial_println("direita");
			else
				serial_println("esquerda");
		}
		else if (BT == 'S')
		{ // Char 'S'teste sensor
		  // testSensors();
		}
		else if (BT == 'M')
		{ // Char 'M'teste motor com sensor
		  // testMotorsWithSensors();
		}
		else if (BT == '0')
		{
			break;
		}
		else
		{
			if (!(BT == '\r' || BT == '\n' || (BT >= '1' && BT <= '9')))
			{ // Se não for ENTER ou numero altera a estrategia inicial
				estrategia = BT;
			}
		}
	}

	serial_end();

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
					delay(50);
					internal_led(false);
					delay(50);
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

	serial_println("COMECOUUUUU");
	sensor_running = true;

	switch (estrategia)
	{
	case ESTRATEGIA_FRENTAO: // frentao
		serial_println("FRENTAAO");
		inicio_frentao();
		break;

	case ESTRATEGIA_CURVAO: // curvao
		serial_println("CURVÃOO");
		inicio_curvao(direc);
		break;

	case ESTRATEGIA_CURVINHA: // curvinha
		serial_println("CURVINHAA");
		inicio_curvinha(direc);
		break;

		/*
		case ESTRATEGIA_DEFESA: // defesa hehehe
			serial_println("DEFESA");
			inicio_defesa(direc, valueSharpF, valueSharpE, valueSharpD);
			break;
		*/

	case ESTRATEGIA_COSTAS: // costas   - só gira 180º
		serial_println("COSTAS");
		inicio_costas(direc);
		break;

		/*
		case ESTRATEGIA_DEFESA_FAKE: // defesa hehehe
			serial_println("DEFESA FAKE");
			inicio_defesa_fake(direc, valueSharpF, valueSharpE, valueSharpD);
			break;

		case ESTRATEGIA_ESTRATEGI: // vem do latim estrategi
			serial_println("ESTRATEGI");
			inicio_estrategi(direc, direc, valueSharpF, valueSharpE, valueSharpD);
			break;

		case ESTRATEGIA_LOOP: // vai direto pro loop "procurar_padrao"
			serial_println("LOOP");
			break;
		*/

	default: // CÓPIA DO FRENTÃO
		serial_println("default");
		inicio_frentao();
		break;
	}
}

void loop()
{
	switch (estrategia)
	{
		/*
	case ESTRATEGIA_DEFESA:
		procurar_defesa();
		serial_println("loop: procurar_defesa!!");
		break;

	case ESTRATEGIA_COSTAS:
		procurar_padrao(VELOCIDADE_GIRO_DESEMPATE);
		serial_println("loop: procurar_padrao!!");
		break;
		*/
	default:
		procurar_padrao(120);
		serial_println("loop: procurar_padrao!!");
		break;
	}
}