#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10079.h"

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
    U10079 instance;
    instance();
}

namespace {
class solution {
    int64_t n_, max_;
public:
    solution() : n_(), max_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return n_ >= 0;
    }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.n_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.max_;
    return out;
}

solution& solution::operator()()
{
    max_ = n_ * (n_ + 1) / 2 + 1;
    return *this;
}
}

void U10079::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
