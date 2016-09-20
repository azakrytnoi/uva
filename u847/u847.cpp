#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u847.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
	U847 instance;
	instance();
}

void U847::operator()()
{
	int64_t n;

	while (std::cin >> n)
	{
		long double lg = std::logl(long double(n)) / std::logl(18);
		int64_t lo = int64_t(std::floorl(lg));
		if (lg - lo < 0.00000000001) { std::cout << "Ollie wins.\n"; continue; }
		if (n / std::powl(18.0, long double(lo)) > 9.0) std::cout << "Ollie wins.\n";
		else std::cout << "Stan wins.\n";
	}
}
