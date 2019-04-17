#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10104.h"

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
    U10104 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : a_(0), b_(0), x_(0), y_(0), d_(0) {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        int64_t a_;
        int64_t b_;
        int64_t x_;
        int64_t y_;
        int64_t d_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.a_ >> sol.b_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.x_ << ' ' << sol.y_ << ' ' << sol.d_;
        return out;
    }

    solution& solution::operator()()
    {
        x_ = 1;
        y_ = 0;
        d_ = a_;
        int64_t x(0), y(1);

        while (b_)
        {
            int64_t quot = d_ / b_;
            int64_t rem = d_ % b_;
            d_ = b_;
            b_ = rem;

            int64_t tmp = x;
            x = x_ - quot * x;
            x_ = tmp;

            tmp = y;
            y = y_ - quot * y;
            y_ = tmp;
        }

        return *this;
    }

}

void U10104::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
