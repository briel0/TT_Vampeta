#include "FS.h"
#include "LittleFS.h"
#include "storage.hpp"

namespace tt::storage
{
	File storage_buffer[16];

	void setup()
	{
		LittleFS.begin();
		for (int i = 0; i < 16; i++)
		{
			// storage_buffer[i] = nullptr;
		}
	}

	storage_t open(const char *name, const char mode)
	{
		storage_t storage = {0, mode};
		File file = LittleFS.open(name, FILE_WRITE);
		return storage;
	}
}