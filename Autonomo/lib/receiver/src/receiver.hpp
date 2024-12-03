#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace receiver
	{
		void setup();
		bool decode();
		void resume();
		uint16_t command();
		bool signal(uint16_t sig);
	}
}