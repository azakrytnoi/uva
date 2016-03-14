#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u424.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

#include "bigint.h"

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U424 instance;
	instance();
}

void U424::operator()()
{
	std::string line;
	math::BigInteger bi;
	while (std::getline(std::cin, line) && line != "0") {
		bi += line;
	}
	std::cout << bi << std::endl;
}