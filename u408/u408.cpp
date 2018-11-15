#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u408.h"

#include "gcd.h"

#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>

U408::U408()
{}

U408::~U408()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U408 instance;
    instance();
}
void U408::operator()() const
{
    uint32_t step, mod;

    while (std::cin >> step >> mod) {
        std::cout << std::setw(10) << step << std::setw(10) << mod << "    ";

        if (math::gcd(mod, step) == 1) {
            std::cout << "Good Choice" << std::endl;

        } else {
            std::cout << "Bad Choice" << std::endl;
        }

        std::cout << std::endl;
    }
}
