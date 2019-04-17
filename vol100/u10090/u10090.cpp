#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10090.h"

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
    U10090 instance;
    instance();
}

namespace {
    class solution {
        int64_t x_, y_, d_;
        int32_t n_, c1_, c2_, n1_, n2_;
        bool fail_;
    public:
        solution() : x_(), y_(), d_(), n_(), c1_(), c2_(), n1_(), n2_(), fail_() {};

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return n_ != 0;
        }
        solution& operator()();

    private:
        void extEuclid(int64_t a, int64_t b)
        {
            if (b == 0)
            {
                x_ = 1;
                y_ = 0;
                d_ = a;
                return;
            }

            extEuclid(b, a % b);
            x_ = x_ - (a / b) * y_;
            std::swap(x_, y_);
        }
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_;

        if (sol.n_ != 0)
        {
            in >> sol.c1_ >> sol.n1_ >> sol.c2_ >> sol.n2_;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.fail_)
        {
            out << "failed";

        }
        else
        {
            out << sol.x_ << ' ' << sol.y_;
        }

        return out;
    }

    solution& solution::operator()()
    {
        extEuclid(n1_, n2_);
        fail_ = (n_ % d_) != 0;

        if (!fail_)
        {
            x_ *= (n_ / d_);
            y_ *= (n_ / d_);
            int64_t a = n2_ / d_, b = n1_ / d_;
            int64_t lf = (int64_t)std::ceil(-x_ / (double)a), rg = (int64_t)std::floor(y_ / (double)b);

            if (lf > rg)
            {
                fail_ = true;

            }
            else
            {
                if (c1_ * (x_ + a * lf) + c2_ * (y_ - b * lf) < c1_ * (x_ + a * rg) + c2_ * (y_ - b * rg))
                {
                    x_ += a * lf, y_ -= b * lf;

                }
                else
                {
                    x_ += a * rg, y_ -= b * rg;
                }
            }
        }

        return *this;
    }
}

void U10090::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
