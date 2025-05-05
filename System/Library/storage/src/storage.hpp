#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace storage
	{
		void setup();
		void close();

		void write_buffer(const char *path, const uint8_t *buffer, const size_t len);
		void append_buffer(const char *path, const uint8_t *buffer, const size_t len);
		void read_buffer(const char *path, uint8_t *buffer, const size_t len);

		template <typename T>
		void write(const char *path, const T data);
		template <typename T>
		void append(const char *path, const T data);
		template <typename T>
		T read(const char *path);
	}
}