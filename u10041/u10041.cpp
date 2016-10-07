#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10041.h"

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
    U10041 instance;
    instance();
}

namespace
{

class solution
{
public:
    solution();

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    solution& operator()();

private:
    std::vector<int32_t> addresses_;
    uint64_t result_;
};

solution::solution() : addresses_(), result_(0)
{}

std::istream& operator >>(std::istream& in, solution& sol)
{
    sol.result_ = 0;
    size_t n;
    in >> n;
    sol.addresses_.clear();
    sol.addresses_.reserve(n);
    std::generate_n(std::back_inserter(sol.addresses_), n, [&]() {
        static int32_t a;
        in >> a;
        return a;
    });
    return in;
}

std::ostream& operator <<(std::ostream& out, const solution& sol)
{
    out << sol.result_;
    return out;
}

solution& solution::operator()()
{
    std::nth_element(addresses_.begin(), addresses_.begin() + addresses_.size() / 2, addresses_.end());
    auto median = addresses_[addresses_.size() / 2];
    result_ = std::accumulate(addresses_.begin(), addresses_.end(), 0, [&](auto current, auto a) {
        return std::abs(median - a) + current;
    });
    return *this;
}

}

void U10041::operator()() const
{
    solution sol;
    size_t N;
    std::cin >> N;
    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
