#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10127.h"

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
    U10127 instance;
    instance();
}

namespace {
class solution {
public:
    solution() : N_(), digits_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    solution& operator ()();

private:
    int64_t N_;
    int32_t digits_;
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.N_;
    sol.digits_ = 0;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.digits_;
    return out;
}

solution& solution::operator()()
{
    int32_t modulo(0);

    do {
        modulo = (modulo * 10 + 1) % N_;
        digits_++;
    } while (modulo != 0);

    return *this;
}
}

void U10127::operator()() const
{
    solution sol;

    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
