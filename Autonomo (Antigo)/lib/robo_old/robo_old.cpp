#include "core.hpp"
#include "robo.hpp"

void inicio_frentao()
{
	forward(255, 255);
	delay(325);
}

void inicio_curvao(Direction init_direc)
{
	if (init_direc == dir)
	{
		right(255, 255);
		delay(96);
		forward(255, 170);
		delay(620);
		left(255, 255);
		delay(100);
		init_direc = esq;
	}
	else
	{
		left(255, 255);
		delay(80);
		forward(170, 255);
		delay(600);
		right(255, 255);
		delay(100);
		init_direc = dir;
	}
}

void inicio_curvinha(Direction init_direc)
{
	if (init_direc == dir)
	{
		right(255, 255);
		delay(96);
		forward(255, 170);
		delay(320);
		left(255, 255);
		delay(100);
		init_direc = esq;
	}
	else
	{
		left(255, 255);
		delay(80);
		forward(170, 255);
		delay(300);
		right(255, 255);
		delay(100);
		init_direc = dir;
	}
}

void inicio_defesa(Direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	int estrategia_defesa_count = 0;
	int estrategia_defesa_timer = 0;
	int estrategia_defesa_editor = 1;

	const uint32_t VELOCIDADE_DEFESA_MAX = 70, VELOCIDADE_DEFESA_MIN = 24;
	uint32_t velocidade_esquerda, velocidade_direita;

	if (init_direc == dir)
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

void inicio_costas(Direction init_direc)
{
	if (init_direc == dir)
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

void inicio_defesa_fake(Direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
{
	int estrategia_defesa_count = 0;
	int estrategia_defesa_timer = 0;
	int estrategia_defesa_editor = 1;

	const uint32_t VELOCIDADE_DEFESA_MAX = 70, VELOCIDADE_DEFESA_MIN = 24;
	uint32_t velocidade_esquerda, velocidade_direita;

	if (init_direc == dir)
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

	if (init_direc == dir)
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

void inicio_estrategi(Direction init_direc, Direction &direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD)
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
		if (init_direc == dir)
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
