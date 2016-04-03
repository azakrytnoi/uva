#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11854.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <numeric>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11854 instance;
    instance();
}
void U11854::operator()()
{
    std::vector<uint32_t> triangle;
    triangle.resize(3);
    while (std::cin) {
        std::cin >> triangle[0] >> triangle[1] >> triangle[2];
        if (std::accumulate(triangle.begin(), triangle.end(), 0) == 0) {
            break;
        }
        std::sort(triangle.begin(), triangle.end());
        std::cout << (triangle[0] * triangle[0] + triangle[1] * triangle[1] == triangle[2] * triangle[2] ? "right" : "wrong") << std::endl;
    }
}