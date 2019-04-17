#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10050.h"

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
    U10050 instance;
    instance();
}

namespace {

    class solution {
        std::vector<uint32_t> hParty_;
        uint32_t lostDays_;
        uint32_t nDays_;
    public:
        solution() : hParty_(), lostDays_(0), nDays_(0) {}

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            sol.lostDays_ = 0;
            uint32_t nParties;
            in  >> sol.nDays_ >> nParties;
            sol.hParty_.clear();
            sol.hParty_.reserve(nParties);
            std::generate_n(std::back_inserter(sol.hParty_), nParties, [&]()
            {
                uint32_t period;
                in >> period;
                return period;
            });
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol)
        {
            out << sol.lostDays_;
            return out;
        }

        solution& operator()()
        {
            for (uint32_t day = 1; day <= nDays_; day++)
            {
                if (day % 7 == 6 || day % 7 == 0)
                {
                    continue;
                }

                bool strike = std::accumulate(hParty_.begin(), hParty_.end(), false, [&](auto prev, auto h)
                {
                    return prev || (day % h == 0);
                });

                if (strike)
                {
                    lostDays_++;
                }
            }

            return *this;
        }

    private:
    };

}

void U10050::operator()() const
{
    uint32_t nCases;
    std::cin >> nCases;
    solution sol;

    while (nCases--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
