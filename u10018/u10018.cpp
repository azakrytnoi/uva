#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10018.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10018 instance;
    instance();
}

namespace
{
class solution
{
public:
    solution() :number_(), count_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    solution& operator ()();

private:
    int64_t number_;
    int64_t count_;

    static int64_t revert(int64_t number);
};

int64_t solution::revert(int64_t number)
{
    int64_t rev(0);
    while (number > 0) {
        rev *= 10;
        rev += number % 10;
        number /= 10;
    }

    return rev;
}

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.number_ = sol.count_ = 0;
    in >> sol.number_;
    return in;
}

std::ostream& operator <<(std::ostream& out, const solution& sol)
{
    out << sol.count_ << ' ' << sol.number_;
    return out;
}

solution& solution::operator()()
{
    int64_t rev(revert(number_));
    do {
        count_++;
        number_ += rev;
    } while ((rev = revert(number_)) != number_);

    return *this;
}
}

void U10018::operator()()
{
    int32_t N;
    std::cin >> N;
    solution sol;
    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
