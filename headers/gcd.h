#pragma once

#include <algorithm>

namespace math
{
	template<typename T>
	T gcd(T a, T b)
	{
		if (a < b) {
			std::swap(a, b);
		}
		T t = a % b;
		if (t == 0) {
			return b;
		}
		return gcd(b, t);
	}

	template<typename T>
	T lcm(T a, T b)
	{
		return (a * b) / gcd(a, b);
	}

	template<typename TL = uint64_t, typename TS = uint32_t>
	TS mod_pow(TL base, TS exp, TS mod)
	{
		TS result = 1;
		while (exp > 0) {
			if (exp % 2 == 1) {
				result = (result * base) % mod;
			}
			exp = exp >> 1;
			base = (base * base) % mod;
		}
		return result;
	}
}
