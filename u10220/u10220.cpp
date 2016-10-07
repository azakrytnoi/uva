#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10220.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

U10220::U10220()
{}

U10220::~U10220()
{}

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10220 instance;
    instance();
}
void U10220::operator()() const
{
    std::vector<int32_t> sums;
    sums.reserve(1001);
    {
        std::vector<int32_t> digits;
        digits.resize(3000);
        digits[0] = 1;
        int32_t remainder(0);
        sums.push_back(0);
        std::generate_n(std::back_inserter(sums), 1000, [&]() -> int32_t {
            uint32_t digit = uint32_t(sums.size());
            std::for_each(digits.begin(), digits.end(), [&](auto& dig)
            {
                remainder += dig * digit;
                dig = remainder % 10;
                // cppcheck-suppress unreadVariable
                remainder /= 10;
            });
            return std::accumulate(digits.begin(), digits.end(), 0); });
    }
    int n;
    while (std::cin >> n) {
        std::cout << sums[n] << std::endl;
    }
}
