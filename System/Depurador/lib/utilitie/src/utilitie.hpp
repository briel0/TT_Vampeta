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

		template <class T>
		void max(T &a, T &b)
		{
			return (a > b ? a : b);
		}
		template <class T, size_t N>
		void max(T (&v)[N])
		{
			T e = v[0];
			for (size_t i = 0; i < N; ++i)
				e = max(e, v[i]);
			return e;
		}

		template <class T>
		void min(T &a, T &b)
		{
			return (a < b ? a : b);
		}
		template <class T, size_t N>
		void min(T (&v)[N])
		{
			T e = v[0];
			for (size_t i = 0; i < N; ++i)
				e = min(e, v[i]);
			return e;
		}
	}
}