#include <cstdint>
#include <cstddef>

namespace tt
{
	namespace utilitie
	{
		template <class T>
		void swap(T &a, T &b)
		{
			T c(std::move(a));
			a = std::move(b);
			b = std::move(c);
		}
		template <class T, size_t N>
		void swap(T (&a)[N], T (&b)[N])
		{
			for (size_t i = 0; i < N; ++i)
				swap(a[i], b[i]);
		}
	}
}