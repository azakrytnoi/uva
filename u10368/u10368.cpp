#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10368.h"

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
    U10368 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t(): o_(), s_(), winner_() {}

        operator bool()
        {
            return o_ != 0 && s_ != 0;
        }

        solution_t& operator()()
        {
            if (o_ > s_) {
                std::swap(o_, s_);
            }

            winner_ = dfs(o_, s_);
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            sol.winner_ = sol.o_ = sol.s_ = 0;
            in >> sol.o_ >> sol.s_;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << (sol.winner_ == 0 ? "Ollie" : "Stan") << " wins";
            return out;
        }

    private:
        uint64_t o_, s_, winner_;

        uint64_t dfs(uint64_t lhs, uint64_t rhs)
        {
            if (lhs == 0) {
                return 0;
            }

            uint64_t res(0);
            res |= !dfs(rhs % lhs, lhs);

            if (rhs / lhs != 1 && res == 0) {
                res |= !dfs(rhs, rhs + rhs % lhs);
            }

            return res;
        }
    };

}

void U10368::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
