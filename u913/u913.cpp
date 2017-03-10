#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u913.h"

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
    U913 instance;
    instance();
}


void U913::operator()() const
{
    uint64_t num;

    while (std::cin >> num) {
        num = num / 2 + 1;
        num *= num;
        std::cout << ((num * 2 - 1) * 3 - 6) << std::endl;
    }
}
