/* ###### DEFINE ###### */
#define DECODE_SONY
/* ###### DEFINE ###### */

/* ###### INCLUDE ###### */
#include <BluetoothSerial.h>
#include <IRremote.hpp>
#include <Arduino.h>
/* ###### INCLUDE ###### */

/* ###### DEFINE ###### */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
/* ###### DEFINE ###### */

/* ###### DEFINE ###### */
#define sensorF 39
#define sensorE 34
#define sensorD 36
/* ###### DEFINE ###### */

/* ###### DEFINE ###### */
#define pwmB 4
#define b1 17
#define b2 16
#define stby 5
#define a1 18
#define a2 19
#define pwmA 23
/* ###### DEFINE ###### */

/* ###### DEFINE ###### */
#define led 2
#define IR 13
/* ###### DEFINE ###### */

/* ###### TYPES ###### */
enum Direction
{
	Esquerda,
	Direita,
};

class Core
{
public:
	static Direction direction;
	TaskHandle_t SensorTask;

	BluetoothSerial SerialBT;
	char BT = 'f';		   // char que é recebida pelo bluetooth
	char estrategia = 'f'; // estratégio de início da luta (qualquer caracter)

	bool loop_procurar_padrao = false;
	static bool running;
	static bool valueSharpF, valueSharpE, valueSharpD;

public:
	void begin();
	virtual void setup() = 0;
	virtual void loop() = 0;

	static void stop();
	static void move(uint32_t pa, uint32_t pb);
	static void left(uint32_t pa, uint32_t pb);
	static void right(uint32_t pa, uint32_t pb);
	static void forward(uint32_t pa, uint32_t pb);
	static void backward(uint32_t pa, uint32_t pb);
	void procurar_defesa();
	void procurar_padrao(uint32_t velocidade_giro);

	void static sensor_task(void *pvParameters);
	void test_sensors();
	void test_wotors_wth_sensors();

private:
};
/* ###### TYPES ###### */