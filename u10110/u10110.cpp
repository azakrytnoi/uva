#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10110.h"

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
    U10110 instance;
    instance();
}

namespace {
class solution {
    uint32_t n_;
    bool light_;
public:
    solution() : n_(), light_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return n_ != 0;
    }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.n_ = 0;
    in >> sol.n_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << (sol.light_ ? "yes" : "no");
    return out;
}

solution& solution::operator()()
{
    uint32_t nsq = std::sqrt(n_);
    light_ = (n_ == (nsq * nsq));
    return *this;
}
}

void U10110::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
