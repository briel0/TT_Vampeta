#include <Arduino.h>
#include "engine.hpp"
#include "internal.hpp"
#include "sensor.hpp"
#include "strategy.hpp"

#include <BluetoothSerial.h>
#include <IRremote.hpp>

#define IR 13

BluetoothSerial SerialBT;
TaskHandle_t SensorTask;

char BT = 'f';		   // char que é recebida pelo bluetooth
char estrategia = 'f'; // estratégio de início da luta (qualquer caracter)
bool sensor_running = false;

direction direc = left;

void sensor_task(void *pvParameters)
{
	while (true)
	{
		if (sensor_running)
		{
			if (IrReceiver.decode())
			{
				IrReceiver.resume();
				if (IrReceiver.decodedIRData.command == 0x2)
				{
					internal_led(true);
					engine_stop();
					ESP.restart();
				}
			}

			/*
			sensor_t sensor = sensor_create_snapshot();
			if (sensor.left)
			{
				direc = esq;
			}
			else if (sensor.right)
			{
				direc = dir;
			}
			*/
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
	IrReceiver.begin(IR, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
	printActiveIRProtocols(&Serial);

	SerialBT.begin(ROBO_NAME);
	SerialBT.println("LIGOUUUU");
	SerialBT.printf("Digite '%c' para Mostrar os Comandos Disponíveis!!\n", COMMAND_HELP);

	xTaskCreatePinnedToCore(sensor_task, "SensorTask", 10000, NULL, 1, &SensorTask, 0);
	delay(500);

	while (BT != '0')
	{ // Inicio quando bluetooth recebe o char '0'
		if (SerialBT.available())
		{
			BT = SerialBT.read();
			SerialBT.println(BT);
		}

		if (BT == 'H')
		{
			SerialBT.printf(STRLN("UM HELP PRA TU!"));
			SerialBT.printf(STRLN("Todos os Comandos (CARACTER MAIUSCULA):"));
			SerialBT.printf(STRLN("\"Catacter do Comando\" -> \"Nome do Comando\""));
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), COMMAND_HELP, "HELP");
			SerialBT.printf(STRLN("Todas as Estratégias: (CARACTER MINUSCULA):"));
			SerialBT.printf(STRLN("\"Caracter da Estrategia\" -> \"Nome da Estratégia\""));
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_FRENTAO, "FRENTÃO");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVAO, "CURVÃO");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_CURVINHA, "CURVINHA");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_DEFESA, "DEFESA");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_COSTAS, "COSTAS");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_DEFESA_FAKE, "DEFESA FAKE");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_ESTRATEGI, "ESTRATEGI");
			SerialBT.printf(STRLN("\"%c\" -> \"%s\""), ESTRATEGIA_LOOP, "Sem estratégia, apenas um loop");
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
			SerialBT.println("Check");
			SerialBT.print("Estrategia: ");
			SerialBT.println(estrategia);
			SerialBT.print("Direc: ");
			if (direc == right)
				SerialBT.println("direita");
			else
				SerialBT.println("esquerda");
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

	SerialBT.end();

	bool ready = false;
	while (true)
	{
		if (IrReceiver.decode())
		{
			IrReceiver.resume();
			if (IrReceiver.decodedIRData.command == 0x0)
			{
				ready = true;
				for (int i = 0; i < 3; i++)
				{
					internal_led(true);
					delay(50);
					internal_led(false);
					delay(50);
					if (IrReceiver.decode())
					{
						IrReceiver.resume();
						if (IrReceiver.decodedIRData.command == 0x1 && ready)
						{
							internal_led(false);
							engine_standby(false);
							break;
						}
					}
				}
			}

			if (IrReceiver.decodedIRData.command == 0x1 && ready)
			{
				internal_led(false);
				engine_standby(false);
				break;
			}
		}
	}

	DEBUGAR_LN("COMECOUUUUU");
	sensor_running = true;

	switch (estrategia)
	{
	case ESTRATEGIA_FRENTAO: // frentao
		DEBUGAR_LN("FRENTAAO");
		inicio_frentao();
		break;

	case ESTRATEGIA_CURVAO: // curvao
		DEBUGAR_LN("CURVÃOO");
		inicio_curvao(direc);
		break;

	case ESTRATEGIA_CURVINHA: // curvinha
		DEBUGAR_LN("CURVINHAA");
		inicio_curvinha(direc);
		break;

		/*
		case ESTRATEGIA_DEFESA: // defesa hehehe
			DEBUGAR_LN("DEFESA");
			inicio_defesa(direc, valueSharpF, valueSharpE, valueSharpD);
			break;
		*/

	case ESTRATEGIA_COSTAS: // costas   - só gira 180º
		DEBUGAR_LN("COSTAS");
		inicio_costas(direc);
		break;

		/*
		case ESTRATEGIA_DEFESA_FAKE: // defesa hehehe
			DEBUGAR_LN("DEFESA FAKE");
			inicio_defesa_fake(direc, valueSharpF, valueSharpE, valueSharpD);
			break;

		case ESTRATEGIA_ESTRATEGI: // vem do latim estrategi
			DEBUGAR_LN("ESTRATEGI");
			inicio_estrategi(direc, direc, valueSharpF, valueSharpE, valueSharpD);
			break;

		case ESTRATEGIA_LOOP: // vai direto pro loop "procurar_padrao"
			DEBUGAR_LN("LOOP");
			break;
		*/

	default: // CÓPIA DO FRENTÃO
		DEBUGAR_LN("default");
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
		DEBUGAR_LN("loop: procurar_defesa!!");
		break;

	case ESTRATEGIA_COSTAS:
		procurar_padrao(VELOCIDADE_GIRO_DESEMPATE);
		DEBUGAR_LN("loop: procurar_padrao!!");
		break;
		*/
	default:
		procurar_padrao(VELOCIDADE_GIRO_PADRAO);
		DEBUGAR_LN("loop: procurar_padrao!!");
		break;
	}
}