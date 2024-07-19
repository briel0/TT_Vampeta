/* ###### DEFINE ###### */
#define DECODE_SONY
/* ###### DEFINE ###### */


/* ###### INCLUDE ###### */
#include <Arduino.h>
/* ###### INCLUDE ###### */


/* ###### TYPES ###### */
enum Direction
{
	esq,
	dir,
};

class Core {
public:
	virtual int setup() = 0;
	virtual int loop() = 0;
private:
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
}
/* ###### TYPES ###### */