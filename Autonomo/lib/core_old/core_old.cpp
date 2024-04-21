// https://www.freecodecamp.org/portuguese/news/map-em-c-mais-mais-explicado-com-exemplos/

#define DECODE_SONY

#include <BluetoothSerial.h>
#include <IRremote.hpp>
#include <definitions.hpp>
#include "core.hpp"
#include "robo.hpp"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

char BT = 'f';		   // char que é recebida pelo bluetooth
char estrategia = 'f'; // estratégio de início da luta (qualquer caracter)

#define sensorF 39
#define sensorE 34
#define sensorD 36

#define pwmB 4
#define b1 17
#define b2 16
#define stby 5
#define a1 18
#define a2 19
#define pwmA 23

#define led 2
#define IR 13

Direction direc = esq;

#pragma region motorController
void stop()
{
	digitalWrite(a1, 1);
	digitalWrite(a2, 1);
	digitalWrite(b1, 1);
	digitalWrite(b2, 1);
}
void backward(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 0);
	digitalWrite(b2, 1);
	digitalWrite(a1, 1);
	digitalWrite(a2, 0);
	analogWrite(pwmB, pb + DELTA_ESQUERDA);
	analogWrite(pwmA, pa + DELTA_DIREITA);
}
void forward(uint32_t pa, uint32_t pb)
{

	digitalWrite(b1, 1);
	digitalWrite(b2, 0);
	digitalWrite(a1, 0);
	digitalWrite(a2, 1);
	analogWrite(pwmB, pb + DELTA_ESQUERDA);
	analogWrite(pwmA, pa + DELTA_DIREITA);
}
void right(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 1);
	digitalWrite(b2, 0);
	digitalWrite(a1, 1);
	digitalWrite(a2, 0);
	analogWrite(pwmB, pb + DELTA_ESQUERDA);
	analogWrite(pwmA, pa + DELTA_DIREITA);
}
void left(uint32_t pa, uint32_t pb)
{
	digitalWrite(b1, 0);
	digitalWrite(b2, 1);
	digitalWrite(a1, 0);
	digitalWrite(a2, 1);
	analogWrite(pwmB, pb + DELTA_ESQUERDA);
	analogWrite(pwmA, pa + DELTA_DIREITA);
}
#pragma endregion motorController

TaskHandle_t SensorTask;

bool loop_procurar_padrao = false;
bool running = false;
bool valueSharpF, valueSharpE, valueSharpD;
void FunctionSensorTask(void *pvParameters)
{
	for (;;)
	{
		if (running)
		{
			if (IrReceiver.decode())
			{
				IrReceiver.resume();
				if (IrReceiver.decodedIRData.command == 0x2)
				{
					digitalWrite(led, 1);
					stop();
					ESP.restart();
				}
			}

			valueSharpF = digitalRead(sensorF);
			valueSharpD = digitalRead(sensorD);
			valueSharpE = digitalRead(sensorE);

			if (valueSharpE)
			{
				direc = esq;
			}
			else if (valueSharpD)
			{
				direc = dir;
			}
		}
		vTaskDelay(1);
	}
}

#pragma region testsRegion
void testSensors()
{
	SerialBT.println("Test Sensors");
	running = true;
	for (;;)
	{
		SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueSharpE, valueSharpF, valueSharpD);
		delay(15);
	}
}

void testMotorsWithSensors()
{
	digitalWrite(stby, 1);
	SerialBT.println("Test Motors With Sensors");
	running = true;
	for (;;)
	{
		SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueSharpE, valueSharpF, valueSharpD);

		if (valueSharpF)
			forward(255, 255);
		else if (valueSharpE)
			left(80, 80);
		else if (valueSharpD)
			right(80, 80);
		else
			stop();
	}
}
#pragma endregion testsRegion

void setup()
{
	pinMode(pwmB, OUTPUT);
	pinMode(b1, OUTPUT);
	pinMode(b2, OUTPUT);
	pinMode(stby, OUTPUT);
	pinMode(a1, OUTPUT);
	pinMode(a2, OUTPUT);
	pinMode(pwmA, OUTPUT);
	pinMode(sensorD, INPUT);
	pinMode(sensorE, INPUT);
	pinMode(sensorF, INPUT);
	pinMode(led, OUTPUT);
	digitalWrite(stby, 0);

	IrReceiver.begin(IR, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
	printActiveIRProtocols(&Serial);

	SerialBT.begin(ROBO_NAME);
	SerialBT.println("LIGOUUUU");
	SerialBT.printf("Digite '%c' para Mostrar os Comandos Disponíveis!!\n", COMMAND_HELP);

	xTaskCreatePinnedToCore(FunctionSensorTask, "SensorTask", 10000, NULL, 1, &SensorTask, 0);
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
			direc = esq;
		}
		else if (BT == '>')
		{
			direc = dir;
		}
		else if (BT == 'C')
		{ // Char 'C' checa as estrategias
			SerialBT.println("Check");
			SerialBT.print("Estrategia: ");
			SerialBT.println(estrategia);
			SerialBT.print("Direc: ");
			if (direc == dir)
				SerialBT.println("direita");
			else
				SerialBT.println("esquerda");
		}
		else if (BT == 'S')
		{ // Char 'S'teste sensor
			testSensors();
		}
		else if (BT == 'M')
		{ // Char 'M'teste motor com sensor
			testMotorsWithSensors();
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
					digitalWrite(led, 1);
					delay(50);
					digitalWrite(led, 0);
					delay(50);
					if (IrReceiver.decode())
					{
						IrReceiver.resume();
						if (IrReceiver.decodedIRData.command == 0x1 && ready)
						{
							digitalWrite(led, 0);
							digitalWrite(stby, 1);
							break;
						}
					}
				}
			}

			if (IrReceiver.decodedIRData.command == 0x1 && ready)
			{
				digitalWrite(led, 0);
				digitalWrite(stby, 1);
				break;
			}
		}
	}

	DEBUGAR_LN("COMECOUUUUU");
	running = true;

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

	case ESTRATEGIA_DEFESA: // defesa hehehe
		DEBUGAR_LN("DEFESA");
		inicio_defesa(direc, valueSharpF, valueSharpE, valueSharpD);
		break;

	case ESTRATEGIA_COSTAS: // costas   - só gira 180º
		DEBUGAR_LN("COSTAS");
		inicio_costas(direc);
		break;

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

	default: // CÓPIA DO FRENTÃO
		DEBUGAR_LN("default");
		inicio_frentao();
		break;
	}
}

void procurar_defesa()
{
	if (loop_procurar_padrao)
	{
		procurar_padrao(VELOCIDADE_GIRO_PADRAO);
	}

	if (valueSharpD == true && valueSharpF + valueSharpE == 0)
	{
		forward(80, 50);
	}
	if (valueSharpF == true && valueSharpD + valueSharpE == 0)
	{
		forward(80, 80);
	}
	if (valueSharpE == true && valueSharpD + valueSharpF == 0)
	{
		forward(50, 80);
	}
	else if (valueSharpD + valueSharpF + valueSharpE >= 2)
	{
		loop_procurar_padrao = true;
	}
	else
	{
		if (direc == esq)
			left(VELOCIDADE_GIRO_DEFESA, VELOCIDADE_GIRO_DEFESA);
		else
			right(VELOCIDADE_GIRO_DEFESA, VELOCIDADE_GIRO_DEFESA);
	}
}

void procurar_padrao(uint32_t velocidade_giro)
{
	if (valueSharpF)
	{
		while (valueSharpF)
		{
			forward(255, 255);
		}
	}
	else if (valueSharpE)
	{
		left(velocidade_giro, velocidade_giro);
	}
	else if (valueSharpD)
	{
		right(velocidade_giro, velocidade_giro);
	}
	else
	{
		if (direc == esq)
			left(velocidade_giro, velocidade_giro);
		else
			right(velocidade_giro, velocidade_giro);
	}
}

void loop()
{
	switch (estrategia)
	{
	case ESTRATEGIA_DEFESA:
		procurar_defesa();
		DEBUGAR_LN("loop: procurar_defesa!!");
		break;

	case ESTRATEGIA_COSTAS:
		procurar_padrao(VELOCIDADE_GIRO_DESEMPATE);
		DEBUGAR_LN("loop: procurar_padrao!!");
		break;

	default:
		procurar_padrao(VELOCIDADE_GIRO_PADRAO);
		DEBUGAR_LN("loop: procurar_padrao!!");
		break;
	}
}
