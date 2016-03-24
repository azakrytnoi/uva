#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u147.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cstdlib>
#include <iosfwd>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U147 instance;
    instance();
}

namespace  {

template<size_t N>
class solution {
    static std::vector<int> coins_;
    std::vector<int64_t> counts_;
public:
    solution() : counts_()
    {
        counts_.resize(N + 1);
        counts_[0] = 1;
        for (auto cit = coins_.begin(); cit != coins_.end(); ++cit) {
            for (size_t i = *cit; i <= N; i++) {
                counts_[i] += counts_[i - *cit];
            }
        }
    }

    int64_t operator()(double amt)
    {
        return counts_[size_t(std::round(amt * 100))];
    }

private:
};

template<size_t N>
std::vector<int> solution<N>::coins_ = {5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

}  // namespace

void U147::operator()()
{
    double amt;
    solution<30000> sol;
    while (std::cin >> amt && amt != 0.0) {
        std::cout.setf(std::ios::right);
        std::cout << std::setw(6) << std::setprecision(2) << std::fixed << amt << std::setw(17) << sol(amt) << std::endl;
    }
}
