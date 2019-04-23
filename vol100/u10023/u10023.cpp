#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10023.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

#include "biguint.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10023 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), x_(), y_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        math::uint_big_t x_, y_;

        math::uint_big_t sqrt(math::uint_big_t& n);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        in >> sol.y_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.x_ << std::endl;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        x_ = sqrt(y_);
        return *this;
    }

    math::uint_big_t solution_t::sqrt(math::uint_big_t& n)
    {
        math::uint_big_t a(1);
        math::uint_big_t b(n);

        while (true)
        {
            auto m = (a + b) / 2;

            if (m == a || m == b)
            {
                return m;
            }

            if (n < (m * m))
            {
                b = m;
            }
            else
            {
                a = m;
            }
        }
    }
}

void U10023::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
