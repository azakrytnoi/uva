#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10062.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <map>
#include <string>

namespace {
	std::ostream& operator << (std::ostream& out, const std::pair<char, uint16_t>& p)
	{
		out << int(p.first) << " " << p.second;
		return out;
	}
}

U10062::U10062() {}

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U10062 instance;
	instance();
}
void U10062::operator()()
{
	std::string line;
	while (std::getline(std::cin, line)) {
		std::map<char, uint16_t> freq;
		std::for_each(line.begin(), line.end(), [&](char ch) {
			freq[ch]++;
		});
		std::vector<std::pair<char, uint16_t>> result;
		std::copy(freq.begin(), freq.end(), std::back_inserter(result));
		std::sort(result.begin(), result.end(), [](auto p1, auto p2) {
			if (p1.second == p2.second) {
				return p1.first > p2.first;
			}
			return p1.second < p2.second;
		});
		//        std::ostream_iterator<std::pair<char, uint16_t>> oit(std::cout, "\n");
		//        std::copy(result.begin(), result.end(), oit);
		std::for_each(result.begin(), result.end(), [](const std::pair<char, uint16_t>& p) {
			std::cout << p << std::endl;
		});
		std::cout << std::endl;
	}
}