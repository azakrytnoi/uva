#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10105.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10105 instance;
    instance();
}

namespace
{
int64_t factorial(int64_t n)
{
    return (n > 1) ? (n * factorial(n - 1)) : 1;
}

class solution
{
public:
    solution() : coeff_(), n_(0), result_(0) {};

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    solution& operator ()();

private:
    std::vector<int16_t> coeff_;
    int16_t n_;
    int64_t result_;
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    int16_t k;
    if (in >> sol.n_ >> k) {
        sol.coeff_.clear();
        sol.result_ = 0;
        sol.coeff_.reserve(k);
        std::generate_n(std::back_inserter(sol.coeff_), k, [&]() {
            int16_t t;
            in >> t;
            return t;
        });
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.result_;
    return out;
}

solution& solution::operator()()
{
    result_ = std::accumulate(coeff_.begin(), coeff_.end(), 1ll, [&](int64_t val, int16_t next) {
        val *= factorial(next);
        return val;
    });
    result_ = factorial(n_) / result_;
    return *this;
}
}

void U10105::operator()()
{
    solution sol;
    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
