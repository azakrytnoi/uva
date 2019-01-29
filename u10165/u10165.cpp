#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10165.h"

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
    U10165 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(), S_() {}

        operator bool()
        {
            return N_ != 0;
        }

        solution_t& operator()()
        {
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            uint64_t n;
            in >> n;
            sol.N_ = n;
            sol.S_ = 0;

            while (n--) {
                uint64_t t;
                in >> t;
                sol.S_ ^= t;
            }

            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << (sol.S_ != 0 ? "Yes" : "No");
            return out;
        }

    private:
        uint64_t N_, S_;
    };

}

void U10165::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
