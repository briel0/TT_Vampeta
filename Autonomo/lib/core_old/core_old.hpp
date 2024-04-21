#define DECODE_SONY

#include <Arduino.h>

class Core
{
private:
	void FunctionSensorTask(void *pvParameters);

public:
	void init();
	void close();
	void backward(uint32_t pa, uint32_t pb);
	void forward(uint32_t pa, uint32_t pb);
	void right(uint32_t pa, uint32_t pb);
	void left(uint32_t pa, uint32_t pb);
};