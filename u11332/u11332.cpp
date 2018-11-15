#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11332.h"

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
    U11332 instance;
    instance();
}

namespace {
class solution {
    int64_t in_;
    int64_t g_;
public:
    solution() : in_(), g_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return in_ != 0;
    }
    solution& operator()();

private:
    static int64_t g(int64_t num);
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.in_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.g_;
    return out;
}

solution& solution::operator()()
{
    g_ = g(in_);
    return *this;
}

int64_t solution::g(int64_t num)
{
    if (num < 10) {
        return num;
    }

    int64_t running(0);

    while (num > 0) {
        running += num % 10;
        num /= 10;
    }

    return g(running);
}
}

void U11332::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
