#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u568.h"

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
    U568 instance;
    instance();
}

namespace
{

template<size_t N>
class solution
{
    std::vector<uint32_t> cache_;
public:
    solution() :
        cache_()
    {
        cache_.resize(N + 1);
        cache_[0] = 1;
        int64_t modFact = 1;
        for (size_t i = 1; i <= N; i++) {
            modFact = (modFact * i);
            while (modFact % 10 == 0) {
                modFact /= 10;
            }
            modFact = modFact % 100000;
            cache_[i] = modFact % 10;
        }
    }

    uint32_t operator[](uint32_t n) const
    {
        return cache_[n];
    }
};

}  // namespace

void U568::operator()()
{
    solution<10001> sol;
    uint32_t n;
    while (std::cin >> n) {
        std::cout.setf(std::ios::right);
        std::cout << std::setw(5) << n << " -> " << sol[n] << std::endl;
    }
}
