/* ###### DEFINE ###### */
#define DECODE_SONY
/* ###### DEFINE ###### */


/* ###### INCLUDE ###### */
#include <core.hpp>
/* ###### INCLUDE ###### */


/* ###### TYPES ###### */
class Robo : public Core {
public:
	int setup() override;
	int loop() override;
}
/* ###### TYPES ###### */