#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u701.h"

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
    U701 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution() : P_(), T_(), LLP_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        solution& operator ()();

    private:
        static const long double C_;
        static const long double _2L_;
        int64_t P_;
        long double T_, LLP_;
    };

    const long double solution::_2L_ = std::log(2);
    const long double solution::C_ = std::log(10) / solution::_2L_;

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.P_;
        return in;
    }

    std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        out << int64_t(std::floor(sol.LLP_ + sol.T_ * sol.C_));
        return out;
    }

    solution& solution::operator()()
    {
        long double LP = std::log((long double)(P_)) / _2L_;
        LLP_ = std::log1p((long double)(P_)) / _2L_;
        T_ = std::floor(std::log10((long double)(P_))) + 2;

        for (; std::ceil(LP + T_ * C_) != std::floor(LLP_ + T_ * C_); T_++);

        return *this;
    }
}

void U701::operator()() const
{
    solution sol;

    while (std::cin >> sol)
    {
        std::cout << sol() << std::endl;
    }
}
