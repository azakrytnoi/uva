#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11172.h"

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
    U11172 instance;
    instance();
}

namespace {
class solution {
    int32_t a_, b_;
    char result_;
public:
    solution() : a_(), b_(), result_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
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
    in >> sol.a_ >> sol.b_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.result_;
    return out;
}

solution& solution::operator()()
{
    result_ = (a_ == b_ ? '=' : (a_ < b_ ? '<' : '>'));
    return *this;
}
}

void U11172::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
