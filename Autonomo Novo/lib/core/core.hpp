#define DECODE_SONY

#include <Arduino.h>

class Core {
public:
	virtual int setup() = 0;
	virtual int loop() = 0;
}