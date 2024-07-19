#define DECODE_SONY

#include <core.hpp>

class Robo : public Core {
public:
	int setup() override;
	int loop() override;
}