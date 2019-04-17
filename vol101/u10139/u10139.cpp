#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10139.h"

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
    U10139 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution();

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const;

    private:
        int32_t m_, n_;
    };

    solution::solution() : m_(), n_() {}

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_ >> sol.m_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        static const char* messgess[] =
        {
            " does not divide ",
            " divides "
        };
        out << sol.m_ << messgess[bool(sol)] << sol.n_ << "!";
        return out;
    }

    solution::operator bool() const
    {
        int32_t m(m_), n(n_);

        if (m == 0)
        {
            return true;
        }

        if (n == 0 && m == 1)
        {
            return true;
        }

        for (int32_t f = 2, mf = int32_t(std::sqrt(m) + 1); m > n && f < mf; ++f)
        {
            int32_t c = 0;

            for (; m % f == 0; m /= f, ++c) {}

            for (int32_t nf = f; c > 0 && nf <= n; c -= n / nf, nf *= f) {}

            if (c > 0)
            {
                return false;
            }
        }

        return m <= n;
    }
}

void U10139::operator()() const
{
    solution sol;

    while (std::cin >> sol)
    {
        std::cout << sol << std::endl;
    }
}
