#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10071.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10071 instance;
    instance();
}

namespace {
    class solution {
        int32_t v_, t_;
        double_t d_;
    public:
        solution() : v_(), t_(), d_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.v_ >> sol.t_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << std::ceil(sol.d_);
        return out;
    }

    solution& solution::operator()()
    {
        d_ = 2 * v_ * t_;
        return *this;
    }
}

void U10071::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
