#include <cstdint>
#include <cstddef>

#define SCAPE_STR "\033["
#define SCAPE_CMD(x) (SCAPE_STR x)
#define COLOR_CMD(x) (SCAPE_STR x "m")

namespace tt
{
	namespace serial
	{
		void setup(const char *name);
		bool enabled();
		int available();
		void wait();
		void erase();
		void end();

		int read();
		void write(uint8_t c);
		void flush();

		void ansi_cmd(const char *cmd);
		void color_cmd(const char *cmd);
		void clear();

		void printf(const char *fmt, ...);
		void scanf(const char *fmt, ...);
		String input();

		template <typename T>
		void println(T val);
		template <typename T>
		void print(T val);
	}
}