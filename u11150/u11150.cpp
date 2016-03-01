#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11150.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace {
int calculate (int full, int extra)
{
    if (full != 0) {
        return full + calculate((full + extra) / 3, (full + extra) % 3);
    }
    return 0;
}
}

U11150::U11150() {}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11150 instance;
    instance();
}
void U11150::operator()()
{
    int N;
    while (std::cin >> N) {
        std::cout << calculate (N, ((N & 0x01) == 1 ? 0 : 2)) << std::endl;
    }
}
