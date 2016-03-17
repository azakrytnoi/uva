#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u458.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U458 instance;
    instance();
}


void U458::operator()()
{
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::ostream_iterator<char> oit(std::cout);
        std::transform(line.begin(), line.end(), oit, [](auto ch) {
            return ch - ('\'' - ' ');
        });
        std::cout << std::endl;
    }
}
