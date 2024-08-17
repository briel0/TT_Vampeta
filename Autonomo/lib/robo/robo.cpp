/* Código do ROBO!!!
setup: Código vai rodar uma vez logo que o robo iniciar
loop: Código vai rodar em loop
*/

#include <robo.hpp>

/* ###### SETUP ######*/
void Robo::setup()
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
			direction = Esquerda;
		}
		else if (BT == '>')
		{
			direction = Esquerda;
		}
		else if (BT == 'C')
		{ // Char 'C' checa as estrategias
			SerialBT.println("Check");
			SerialBT.print("Estrategia: ");
			SerialBT.println(estrategia);
			SerialBT.print("Direc: ");
			if (direction == Direita)
				SerialBT.println("direita");
			else
				SerialBT.println("esquerda");
		}
		else if (BT == 'S')
		{ // Char 'S'teste sensor
			test_sensors();
		}
		else if (BT == 'M')
		{ // Char 'M'teste motor com sensor
			test_wotors_wth_sensors();
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
		inicio_curvao(direction);
		break;

	case ESTRATEGIA_CURVINHA: // curvinha
		DEBUGAR_LN("CURVINHAA");
		inicio_curvinha(direction);
		break;

	case ESTRATEGIA_DEFESA: // defesa hehehe
		DEBUGAR_LN("DEFESA");
		inicio_defesa(direction, valueSharpF, valueSharpE, valueSharpD);
		break;

	case ESTRATEGIA_COSTAS: // costas   - só gira 180º
		DEBUGAR_LN("COSTAS");
		inicio_costas(direction);
		break;

	case ESTRATEGIA_DEFESA_FAKE: // defesa hehehe
		DEBUGAR_LN("DEFESA FAKE");
		inicio_defesa_fake(direction, valueSharpF, valueSharpE, valueSharpD);
		break;

	case ESTRATEGIA_ESTRATEGI: // vem do latim estrategi
		DEBUGAR_LN("ESTRATEGI");
		inicio_estrategi(direction, direction, valueSharpF, valueSharpE, valueSharpD);
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
/* ###### SETUP ######*/

/* ###### LOOP ######*/
void Robo::loop()
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
/* ###### LOOP ######*/

void Robo::inicio_frentao()
{
	forward(255, 255);
	delay(325);
}

void Robo::inicio_curvao(Direction init_direc)
{
	if (init_direc == direction)
	{
		right(255, 255);
		delay(96);
		forward(255, 170);
		delay(620);
		left(255, 255);
		delay(100);
		init_direc = Esquerda;
	}
	else
	{
		left(255, 255);
		delay(80);
		forward(170, 255);
		delay(600);
		right(255, 255);
		delay(100);
		init_direc = direction;
	}
}

void Robo::inicio_curvinha(Direction init_direc)
{
	if (init_direc == direction)
	{
		right(255, 255);
		delay(96);
		forward(255, 170);
		delay(320);
		left(255, 255);
		delay(100);
		init_direc = Esquerda;
	}
	else
	{
		left(255, 255);
		delay(80);
		forward(170, 255);
		delay(300);
		right(255, 255);
		delay(100);
		init_direc = direction;
	}
}

void Robo::inicio_defesa(Direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	int estrategia_defesa_count = 0;
	int estrategia_defesa_timer = 0;
	int estrategia_defesa_editor = 1;

	const uint32_t VELOCIDADE_DEFESA_MAX = 70, VELOCIDADE_DEFESA_MIN = 24;
	uint32_t velocidade_esquerda, velocidade_direita;

	if (init_direc == direction)
	{
		velocidade_esquerda = VELOCIDADE_DEFESA_MAX;
		velocidade_direita = VELOCIDADE_DEFESA_MIN;
	}
	else
	{
		velocidade_esquerda = VELOCIDADE_DEFESA_MIN;
		velocidade_direita = VELOCIDADE_DEFESA_MAX;
	}

	forward(velocidade_direita, velocidade_esquerda);
	while ((estrategia_defesa_count < 3 && valueSharpF + valueSharpE + valueSharpD >= 1) || (estrategia_defesa_count < 5))
	{
		estrategia_defesa_timer += estrategia_defesa_editor;
		if (estrategia_defesa_timer == 24 || estrategia_defesa_timer == -24)
		{
			estrategia_defesa_count++;
			estrategia_defesa_editor *= -1;

			int velocidade_temp = velocidade_direita;
			velocidade_direita = velocidade_esquerda;
			velocidade_esquerda = velocidade_temp;
			forward(velocidade_direita, velocidade_esquerda);
		}
		delay(10);
	}
}

void Robo::inicio_costas(Direction init_direc)
{
	if (init_direc == direction)
	{ // prestar atenção na init_direc, pq os robôs estarão de costas
		right(255, 255);
		delay(190);
	}
	else
	{
		left(255, 255);
		delay(190);
	}
}

void Robo::inicio_defesa_fake(Direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	int estrategia_defesa_count = 0;
	int estrategia_defesa_timer = 0;
	int estrategia_defesa_editor = 1;

	const uint32_t VELOCIDADE_DEFESA_MAX = 70, VELOCIDADE_DEFESA_MIN = 24;
	uint32_t velocidade_esquerda, velocidade_direita;

	if (init_direc == direction)
	{
		velocidade_esquerda = VELOCIDADE_DEFESA_MAX;
		velocidade_direita = VELOCIDADE_DEFESA_MIN;
	}
	else
	{
		velocidade_esquerda = VELOCIDADE_DEFESA_MIN;
		velocidade_direita = VELOCIDADE_DEFESA_MAX;
	}

	forward(velocidade_direita, velocidade_esquerda);
	while (estrategia_defesa_count < 3)
	{
		estrategia_defesa_timer += estrategia_defesa_editor;
		if (estrategia_defesa_timer == 24 || estrategia_defesa_timer == -24)
		{
			estrategia_defesa_count += 1;
			estrategia_defesa_editor *= -1;

			int velocidade_temp = velocidade_direita;
			velocidade_direita = velocidade_esquerda;
			velocidade_esquerda = velocidade_temp;
			forward(velocidade_direita, velocidade_esquerda);
		}
		delay(10);
	}

	if (init_direc == direction)
	{
		right(255, 255);
		delay(90);
		forward(255, 70);
		delay(440);
	}
	else
	{
		left(255, 255);
		delay(90);
		forward(70, 255);
		delay(440);
	}
}

void Robo::inicio_estrategi(Direction init_direc, Direction &direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	forward(255, 255);
	delay(125);

	if (valueSharpF + valueSharpE + valueSharpD == 3)
	{
		return;
	}
	else if (valueSharpD + valueSharpE == 2)
	{
		return;
	}
	else if (valueSharpF + valueSharpE == 2)
	{
		forward(255, 150);
		delay(400);
	}
	else if (valueSharpF + valueSharpD == 2)
	{
		forward(150, 255);
		delay(400);
	}
	else if (valueSharpF)
	{
		if (init_direc == direction)
		{
			right(255, 255);
			delay(90);
			forward(255, 70);
			delay(400);
		}
		else
		{
			left(255, 255);
			delay(90);
			forward(70, 255);
			delay(400);
		}
	}
	else if (valueSharpE)
	{
		forward(255, 70);
		delay(400);
	}
	else if (valueSharpD)
	{
		forward(70, 255);
		delay(400);
	}
	else
	{
		return;
	}
}