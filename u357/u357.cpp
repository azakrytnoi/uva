#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u357.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U357 instance;
    instance();
}

namespace
{

template<size_t N>
class solution
{
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

    int64_t operator()(int amt)
    {
        return counts_[size_t(amt)];
    }

private:
};

template<size_t N>
std::vector<int> solution<N>::coins_ = {1, 5, 10, 25, 50 };

}  // namespace

void U357::operator()() const
{
    int amt;
    solution<30000> sol;
    while (std::cin >> amt) {
        std::cout.setf(std::ios::right);
        int64_t res = sol(amt);
        if (res > 1) {
            std::cout << "There are " << res << " ways to produce " << amt << " cents change." << std::endl;
        } else {
            std::cout << "There is only 1 way to produce " << amt << " cents change." << std::endl;
        }
    }
}
