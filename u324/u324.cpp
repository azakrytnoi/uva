#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u324.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <unordered_map>
#include <array>

#include "bigint.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U324 instance;
    instance();
}

namespace {
    class solution {
        std::vector<std::vector<uint16_t>> cache_;
        uint16_t number_;
    public:
        solution() : cache_(), number_(0)
        {
            math::BigInteger fact("1");
            cache_.resize(368);

            for (uint16_t i = 1; i <= 367; i++) {
                cache_[i].resize(10);
                fact *= (int)i;
                bool leading0(true);
                std::for_each(fact.data().rbegin(), fact.data().rend(), [&](auto digit) {
                    if (leading0) {
                        leading0 = digit == 0;
                    }

                    if (!leading0) {
                        cache_[i][digit]++;
                    }
                });
            }
        }

        solution& operator () (uint16_t n)
        {
            number_ = n;
            return *this;
        }

        friend std::ostream& operator << (std::ostream& out, solution& sol);

    private:
    };

    std::ostream& operator << (std::ostream& out, solution& sol)
    {
        out << sol.number_ << "! --";
        uint16_t idx(0);
        std::for_each(sol.cache_[sol.number_].begin(), sol.cache_[sol.number_].end(), [&](auto freq) {
            // cppcheck-suppress knownConditionTrueFalse
            if ((idx % 5) == 0) {
                out << std::endl;
            }

            out << "   (" << (idx++) << ")" << std::setw(5) << freq << " ";
        });
        return out;
    }
} // namespace

void U324::operator()() const
{
    solution sol;
    uint16_t n;

    while (std::cin >> n && n > 0) {
        std::cout << sol(n) << std::endl;
    }
}
