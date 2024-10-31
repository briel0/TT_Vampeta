#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace serial
	{
		void setup(const char *name);
		int available();
		void end();

		int read();
		void write(uint8_t c);
		void printf(const char *fmt, ...);

		template <typename T>
		void println(T val);
		template <typename T>
		void print(T val);
	}
}