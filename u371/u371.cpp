#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u371.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <unordered_map>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U371 instance;
    instance();
}

namespace
{
class solution
{
    std::unordered_map<int64_t, uint32_t> cache_;
public:
    solution() : cache_() {}

    uint32_t func_r(uint64_t x)
    {
        if (cache_[x] != 0) {
            return cache_[x];
        }

        long y = x % 2 ? 3 * x + 1 : x / 2;
        if (y == 1) {
            return 1;
        }
        return cache_[x] = func_r(y) + 1;
    }

    uint32_t func_i(uint64_t nn)
    {
        uint32_t count(0);
        uint64_t n = nn;
        do {
            if (cache_[nn] != 0) {
                return cache_[nn];
            }
            n = (n & 0x01) ? 3 * n + 1 : n >> 1;
            count++;
        } while (n != 1);
        cache_[nn] = count;
        return count;
    }

    std::string operator() (uint32_t l, uint32_t h)
    {
        if (l > h) {
            std::swap(l, h);
        }
        uint64_t result(0);
        uint32_t idx(l);
        for (uint64_t n = l; n <= h; n++) {
            uint32_t tmp = func_i(n);
            if (tmp > result) {
                result = tmp;
                idx = n;
            }
        }
        std::stringstream ss;
        ss << "Between " << l << " and " << h << ", " << idx << " generates the longest sequence of " << result << " values.";
        return ss.str();
    }

private:
};
}  // namespace

void U371::operator()()
{
    solution sol;
    uint32_t l, h;
    while (std::cin >> l >> h && (l + h) != 0) {
        std::cout << sol(l, h) << std::endl;
    }
}
