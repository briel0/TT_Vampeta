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

void inicio_frentao();
void inicio_curvao(Direction init_direc);
void inicio_curvinha(Direction init_direc);
void inicio_defesa(Direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD);
void inicio_costas(Direction init_direc);
void inicio_defesa_fake(Direction init_direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD);
void inicio_estrategi(Direction init_direc, Direction &direc, bool &valueSharpF, bool &valueSharpE, bool &valueSharpD);