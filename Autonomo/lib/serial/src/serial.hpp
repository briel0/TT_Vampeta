#include <BluetoothSerial.h>
#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace serial
	{
		void setup(const char *name);
		BluetoothSerial serial();
		bool enable();
		int available();
		void end();

		int read();
		void write(uint8_t c);
		void printf(const char *fmt, ...);

		template <typename T>
		void println(T val)
		{
			if (!enable())
			{
				return;
			}
			BluetoothSerial SerialBT = serial();
			SerialBT.println(val);
		}

		template <typename T>
		void print(T val)
		{
			if (!enable())
			{
				return;
			}
			BluetoothSerial SerialBT = serial();
			SerialBT.print(val);
		}
	}
}