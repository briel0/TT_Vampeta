#define DECODE_SONY

#include <Arduino.h>

#define DEBUG false
#if DEBUG
#define DEBUGAR_LN(x) SerialBT.println(x)
#define DEBUGAR_F(x) SerialBT.printf(x)
#else
#define DEBUGAR_LN(x)
#define DEBUGAR_F(x)
#endif

#define ROBO_NAME "Arruela"
#define STRLN(x) x "\n"

#define COMMAND_HELP 'H'
#define ESTRATEGIA_FRENTAO 'a'
#define ESTRATEGIA_CURVAO 'b'
#define ESTRATEGIA_CURVINHA 'c'
#define ESTRATEGIA_DEFESA 'd'
#define ESTRATEGIA_COSTAS 'e'
#define ESTRATEGIA_DEFESA_FAKE 'f'
#define ESTRATEGIA_ESTRATEGI 'g'
#define ESTRATEGIA_LOOP 'z'

#define VELOCIDADE_GIRO_PADRAO 130
#define VELOCIDADE_GIRO_DEFESA 110
#define VELOCIDADE_GIRO_DESEMPATE 100

#define DELTA_ESQUERDA 0
#define DELTA_DIREITA 0

enum direction
{
	left,
	right,
};

void inicio_frentao();
void inicio_curvao(direction init_direc);
void inicio_curvinha(direction init_direc);
void inicio_costas(direction init_direc);
void procurar_padrao(uint8_t velocidade_giro);
/*
void inicio_defesa(direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD);
void inicio_defesa_fake(direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD);
void inicio_estrategi(direction init_direc, direction &direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD);
*/