
#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u623.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include <biguint.h>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U623 instance;
    instance();
}

namespace {

    class solution {
        math::uint_big_t int_;
        uint32_t n_;
    public:
        solution() : int_(), n_(0) {}

        friend std::istream& operator >> (std::istream& in, solution& sol)
        {
            in >> sol.n_;
            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const solution& sol)
        {
            out << sol.n_ << "!" << std::endl << sol.int_;
            return out;
        }

        solution& operator()()
        {
            int_ = 1;

            for (math::uint_big_t n = 2; n <= n_; ++n) {
                int_ *= n;
            }

            return *this;
        }
    };

}  // namespace

void U623::operator()() const
{
    solution sol;

    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
