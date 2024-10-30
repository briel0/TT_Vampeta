#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace receiver
	{
		void begin();
		bool decode();
		void resume();
		uint16_t command();
	}
}