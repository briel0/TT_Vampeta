#define DECODE_SONY

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <IRremote.hpp>
#include <definitions.hpp>

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

class Core
{
private:
	BluetoothSerial SerialBT;
	Direction direc = esq;
	char BT = 'f';
	char estrategia = 'f';

	TaskHandle_t SensorTask;

	bool loop_procurar_padrao = false;
	bool running = false;
	bool valueSharpF, valueSharpE, valueSharpD;

	void FunctionSensorTask(void *pvParameters);

public:
	void init();
	void close();
	void stop();
	void backward(uint32_t pa, uint32_t pb);
	void forward(uint32_t pa, uint32_t pb);
	void right(uint32_t pa, uint32_t pb);
	void left(uint32_t pa, uint32_t pb);
};