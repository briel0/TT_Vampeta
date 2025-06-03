#include "FS.h"
#include "LittleFS.h"
#include "storage.hpp"

#pragma region "Size Data Defines"
#ifndef BYTE_SIZE
#define BYTE_SIZE 256
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE (BYTE_SIZE * 2)
#endif
#ifndef STACK_SIZE
#define STACK_SIZE (BYTE_SIZE * 16)
#endif
#pragma endregion "Size Data Defines"

namespace tt::storage
{
	void setup()
	{
		LittleFS.begin();
	}

	void close()
	{
		LittleFS.end();
	}

	void write_buffer(const char *path, const uint8_t *buffer, const size_t len)
	{
		File file = LittleFS.open(path, FILE_WRITE);
		file.seek(SEEK_SET);
		for (size_t i = 0; i < len; i++)
		{
			file.write(buffer[i]);
		}
		file.close();
	}

	void append_buffer(const char *path, const uint8_t *buffer, const size_t len)
	{
		File file = LittleFS.open(path, FILE_WRITE);
		file.seek(SEEK_END);
		for (size_t i = 0; i < len; i++)
		{
			file.write(buffer[i]);
		}
		file.close();
	}

	void read_buffer(const char *path, uint8_t *buffer, const size_t len)
	{
		File file = LittleFS.open(path, FILE_READ);
		file.seek(SEEK_SET);
		for (size_t i = 0; i < min(len, file.size()); i++)
		{
			buffer[i] = file.read();
		}
		file.close();
	}

	template <typename T>
	void write(const char *path, const T data)
	{
		write_buffer(path, (uint8_t *)(&data), sizeof(data));
	}
	template void write(const char *path, const int8_t);
	template void write(const char *path, const int16_t);
	template void write(const char *path, const int32_t);
	template void write(const char *path, const int64_t);
	template void write(const char *path, const uint8_t);
	template void write(const char *path, const uint16_t);
	template void write(const char *path, const uint32_t);
	template void write(const char *path, const uint64_t);
	template void write(const char *path, const long);
	template void write(const char *path, const long long);
	template void write(const char *path, const unsigned long);
	template void write(const char *path, const unsigned long long);
	template void write(const char *path, const signed long);
	template void write(const char *path, const signed long long);
	template void write(const char *path, const float);
	template void write(const char *path, const double);
	template void write(const char *path, const char);
	template void write(const char *path, const wchar_t);

	template <typename T>
	void append(const char *path, const T data)
	{
		append_buffer(path, (uint8_t *)(&data), sizeof(data));
	}
	template void append(const char *path, const int8_t);
	template void append(const char *path, const int16_t);
	template void append(const char *path, const int32_t);
	template void append(const char *path, const int64_t);
	template void append(const char *path, const uint8_t);
	template void append(const char *path, const uint16_t);
	template void append(const char *path, const uint32_t);
	template void append(const char *path, const uint64_t);
	template void append(const char *path, const long);
	template void append(const char *path, const long long);
	template void append(const char *path, const unsigned long);
	template void append(const char *path, const unsigned long long);
	template void append(const char *path, const signed long);
	template void append(const char *path, const signed long long);
	template void append(const char *path, const float);
	template void append(const char *path, const double);
	template void append(const char *path, const char);
	template void append(const char *path, const wchar_t);

	template <typename T>
	T read(const char *path)
	{
		T data;
		read_buffer(path, (uint8_t *)(&data), sizeof(data));
		return data;
	}
	template int8_t read(const char *path);
	template int16_t read(const char *path);
	template int32_t read(const char *path);
	template int64_t read(const char *path);
	template uint8_t read(const char *path);
	template uint16_t read(const char *path);
	template uint32_t read(const char *path);
	template uint64_t read(const char *path);
	template long read(const char *path);
	template long long read(const char *path);
	template unsigned long read(const char *path);
	template unsigned long long read(const char *path);
	template signed long read(const char *path);
	template signed long long read(const char *path);
	template float read(const char *path);
	template double read(const char *path);
	template char read(const char *path);
	template wchar_t read(const char *path);
}