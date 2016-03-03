#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11388.h"

#include <iostream>

U11388::U11388()
{
}

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U11388 instance;
	instance();
}
void U11388::operator()()
{
	int N;
	std::cin >> N;
	while (N--) {
		int gcd, lcm;
		std::cin >> gcd >> lcm;
		if (lcm % gcd == 0) {
			std::cout << gcd << " " << lcm << std::endl;
		}
		else {
			std::cout << "-1" << std::endl;
		}
	}
}