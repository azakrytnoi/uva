#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u350.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <bitset>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U350 instance;
    instance();
}

namespace
{

struct linear {
    uint32_t Z, I, M;
    uint32_t operator ()(uint32_t n) const
    {
        return (Z * n + I) % M;
    }
};

class solution
{
    linear lcm_;
    uint32_t count_;
    uint32_t L;
    std::vector<bool> mask_;
public:
    solution() : lcm_(), count_(0), L(0), mask_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.lcm_.Z >> sol.lcm_.I >> sol.lcm_.M >> sol.L;
        return in;
    }

    operator bool () const
    {
        return !(lcm_.Z == 0 && lcm_.I == 0 && lcm_.M == 0 && L == 0);
    }

    operator uint32_t () const
    {
        return count_;
    }

    solution& operator () ()
    {
        uint32_t n = lcm_(L);
        count_ = 0;
        mask_.clear();
        mask_.resize(lcm_.M + 1);
        while (!mask_[n]) {
            mask_[n] = true;
            n = lcm_(n);
            count_++;
        }
        return *this;
    }

};

}  // namespace

void U350::operator()() const
{
    solution sol;
    uint32_t caseNo(0);
    while (std::cin >> sol && sol) {
        std::cout << "Case " << (++caseNo) << ": " << uint32_t(sol()) << std::endl;
    }
}
