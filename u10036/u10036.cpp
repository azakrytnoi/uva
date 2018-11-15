#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10036.h"

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
    U10036 instance;
    instance();
}

namespace {

class solution {
    int32_t N_, K_;
    bool divisible_;
    std::vector<int32_t> val_;
public:
    solution() : N_(), K_(), divisible_(), val_()
    {
        val_.reserve(10000);
    }

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
    in >> sol.N_ >> sol.K_;
    sol.val_.clear();
    std::generate_n(std::back_inserter(sol.val_), sol.N_, [&]() {
        int32_t tmp;
        in >> tmp;
        return tmp;
    });
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << (sol.divisible_ ? "Divisible" : "Not divisible");
    return out;
}

solution& solution::operator()()
{
    std::vector<std::vector<bool>> remainders(2, std::vector<bool>(K_));
    int32_t iTemp(0), iAnswer(1);
    remainders[iTemp][0] = remainders[iAnswer][0] = true;
    std::for_each(val_.begin(), val_.end(), [&](int32_t val) {
        std::swap(iTemp, iAnswer);
        remainders[iAnswer].assign(K_, false);

        for (int32_t remainder = 0; remainder < K_; remainder++) {
            if (remainders[iTemp][remainder]) {
                remainders[iAnswer][std::abs(remainder + (val % K_)) % K_] = true;
                remainders[iAnswer][std::abs(remainder + (-val % K_)) % K_] = true;
            }
        }
    });
    divisible_ = remainders[iAnswer][0];
    return *this;
}

}

void U10036::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
