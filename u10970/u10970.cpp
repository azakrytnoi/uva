#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10970.h"

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
    U10970 instance;
    instance();
}

namespace {

class solution {
    int32_t n_, m_;
public:
    solution() : n_(), m_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol);
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
    in >> sol.n_ >> sol.m_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << (sol.n_ * sol.m_ - 1);
    return out;
}

solution& solution::operator()()
{
    return *this;
}

}

void U10970::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
