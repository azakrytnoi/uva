#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u12243.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cctype>
#include <sstream>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U12243 instance;
	instance();
}

void U12243::operator()()
{
	std::string line;
	while (std::getline(std::cin, line) && line != "*") {
		std::transform(line.begin(), line.end(), line.begin(), std::tolower);
		char ch = line[0];
		std::stringstream in(line);
		std::istream_iterator<std::string> iss(in);
		bool tautogram = std::accumulate(iss, std::istream_iterator<std::string>(), true, [&](auto init, auto word) { return init && word[0] == ch; });
		std::cout << (tautogram ? "Y" : "N") << std::endl;
	}
}