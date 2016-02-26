#define UVA_API_EXPORT __declspec(dllexport)

#include "u100.h"

#include <iostream>
#include <limits>
#include <algorithm>

U100::U100()
{
}


U100::~U100()
{
}

void U100::operator()()
{
    uint32_t i, j;
    while (std::cin >> i >> j) {
        int start(i), end(j);
        if (start > end) std::swap(start, end);
        std::cout << i << " " << j << " " << cycle_ength(start, end, [](uint32_t n) -> uint32_t {
            uint32_t cnt(1);
            while (n != 1)
            {
                cnt++;
                if (n & 0x01) {
                    n = 3 * n + 1;
                } else {
                    n /= 2;
                }
            }
            return cnt;
        }) << std::endl;
    }
}

uint32_t U100::cycle_ength(uint32_t start, uint32_t end, std::function<uint32_t(uint32_t)> f)
{
    uint32_t result(std::numeric_limits<uint32_t>::min());
    for (uint32_t n = start; n <= end; n++) {
        result = std::max(f(n), result);
    }
    return result;
}
