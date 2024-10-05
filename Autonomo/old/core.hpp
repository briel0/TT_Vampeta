#define DECODE_SONY

#include <Arduino.h>

enum Direction
{
	esq,
	dir,
};

void stop();
void backward(uint32_t pa, uint32_t pb);
void forward(uint32_t pa, uint32_t pb);
void right(uint32_t pa, uint32_t pb);
void left(uint32_t pa, uint32_t pb);
void FunctionSensorTask(void *pvParameters);
void testSensors();
void testMotorsWithSensors();
void setup();
void procurar_defesa();
void procurar_padrao(uint32_t velocidade_giro);
void loop();