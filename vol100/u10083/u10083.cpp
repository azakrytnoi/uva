#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10083.h"

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
#include <cmath>

#include "biguint.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10083 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : t_(), a_(), b_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        uint32_t t_, a_, b_;
        std::pair<bool, math::uint_big_t> result_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.t_ >> sol.a_ >> sol.b_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "(" << sol.t_ << '^' << sol.a_ << "-1)/(" << sol.t_ << '^' << sol.b_ << "-1) ";

        if (sol.result_.first)
        {
            out << sol.result_.second;
        }
        else
        {
            out << "is not an integer with less than 100 digits.";
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        if (t_ == 1 || a_ % b_ != 0 || (a_ - b_) * std::log10(t_) > 99)
        {
            result_.first = false;
        }
        else if (a_ == b_)
        {
            result_ = {true, 1};
        }
        else
        {
            math::uint_big_t a(t_), b(t_);
            a ^= a_, b ^= b_;
            a -= 1, b -= 1;

            if (b == 0)
            {
                result_.first = false;
            }
            else
            {
                result_ = {true, a / b};
            }
        }

        return *this;
    }

}

void U10083::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
