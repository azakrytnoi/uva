#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11311.h"

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
    U11311 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), m_(), n_(), r_(), c_() {}

        operator bool()
        {
            return N_-- > 0;
        }

        solution_t& operator()()
        {
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            if (sol.N_ == std::numeric_limits<size_t>::max())
            {
                in >> sol.N_;
            }

            in >> sol.m_ >> sol.n_ >> sol.r_ >> sol.c_;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << (((sol.m_ - sol.r_ - 1) ^ (sol.n_ - sol.c_ - 1) ^ sol.r_ ^ sol.c_) != 0 ? "Gretel" : "Hansel");
            return out;
        }

    private:
        size_t N_;
        size_t m_, n_, r_, c_;
    };

}

void U11311::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
