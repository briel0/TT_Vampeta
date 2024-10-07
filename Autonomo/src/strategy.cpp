#include <Arduino.h>
#include "strategy.hpp"
#include "engine.hpp"
#include "internal.hpp"
#include "sensor.hpp"

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
		delay(96);
		engine_move(ENGINE_FRONT_FULL, ENGINE_FRONT(170));
		delay(620);
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(100);
		init_direc = left;
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(80);
		engine_move(ENGINE_FRONT(170), ENGINE_FRONT_FULL);
		delay(600);
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(100);
		init_direc = right;
	}
}

void inicio_curvinha(direction init_direc)
{
	if (init_direc == right)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(96);
		engine_move(ENGINE_FRONT_FULL, ENGINE_FRONT(170));
		delay(320);
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(100);
		init_direc = left;
	}
	else
	{
		engine_move(ENGINE_BACK_FULL, ENGINE_FRONT_FULL);
		delay(80);
		engine_move(ENGINE_FRONT(170), ENGINE_FRONT_FULL);
		delay(300);
		engine_move(ENGINE_FRONT_FULL, ENGINE_BACK_FULL);
		delay(100);
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
	sensor_t sensor = sensor_create_snapshot();
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

/*
void inicio_defesa(direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	int estrategia_defesa_count = 0;
	int estrategia_defesa_timer = 0;
	int estrategia_defesa_editor = 1;

	const uint32_t VELOCIDADE_DEFESA_MAX = 70, VELOCIDADE_DEFESA_MIN = 24;
	uint32_t velocidade_esquerda, velocidade_direita;

	if (init_direc == right)
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

void inicio_defesa_fake(direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	int estrategia_defesa_count = 0;
	int estrategia_defesa_timer = 0;
	int estrategia_defesa_editor = 1;

	const uint32_t VELOCIDADE_DEFESA_MAX = 70, VELOCIDADE_DEFESA_MIN = 24;
	uint32_t velocidade_esquerda, velocidade_direita;

	if (init_direc == right)
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

	if (init_direc == right)
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

void inicio_estrategi(direction init_direc, direction &direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
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
		if (init_direc == right)
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
*/