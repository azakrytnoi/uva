
#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u495.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include "fibonacci.h"
#include <biguint.h>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U495 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : fb_(), n_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        math::fibonacci<5001, math::uint_big_t> fb_;
        size_t n_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "The Fibonacci number for " << sol.n_ << " is " << sol.fb_[sol.n_];
        return out;
    }

    solution_t& solution_t::operator()()
    {
        return *this;
    }

}

void U495::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
