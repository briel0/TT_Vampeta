#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace storage
	{
		struct storage_t
		{
			uint8_t id;
			char mode;
		};
		void setup();
		storage_t open(const char *name, const char mode);
	}
	typedef storage::storage_t storage_t;
}