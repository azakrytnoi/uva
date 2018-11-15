#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10128.h"

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
    U10128 instance;
    instance();
}

namespace {
constexpr size_t N = 14;
class solution {
    std::vector<std::vector<std::vector<int64_t>>> dp_;
    int32_t n_, p_, r_;
public:
    solution();

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return true;
    }
    solution& operator()();

private:
};

solution::solution() : dp_(N, std::vector<std::vector<int64_t>>(N, std::vector<int64_t>(N))),
    n_(), p_(), r_()
{
    std::vector<std::vector<int64_t>> pascalTriangle(N, std::vector<int64_t>(N));
    pascalTriangle[0][0] = 1;

    for (size_t i = 1; i < N; i++) {
        pascalTriangle[i][0] = pascalTriangle[i][i] = 1;
    }

    for (size_t i = 2; i < N; ++i) {
        for (size_t j = 1; j < i; ++j) {
            pascalTriangle[i][j] = pascalTriangle[i - 1][j] + pascalTriangle[i - 1][j - 1];
        }
    }

    dp_[1][1][1] = dp_[0][0][0] = 1;

    for (size_t n = 2; n < 14; ++n) {
        for (size_t p = 1; p <= n; ++p) {
            for (size_t r = 1; r <= p; ++r) {
                for (size_t i = p - 1; i <= n - 1 - (r - 1); ++i) {
                    dp_[n][p][r] +=
                        pascalTriangle[n - 1][i] *
                        std::accumulate(dp_[i][p - 1].begin(), dp_[i][p - 1].end(), 0ll) *
                        std::accumulate(dp_[n - 1 - i][r - 1].begin(), dp_[n - 1 - i][r - 1].end(), 0ll);
                    dp_[n][r][p] = dp_[n][p][r];  // symmetry
                }
            }
        }
    }
}

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.n_ >> sol.p_ >> sol.r_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    if (sol.p_ == 0 || sol.r_ == 0 || sol.p_ + sol.r_ > sol.n_ + 1) {
        out << 0;

    } else {
        out << sol.dp_[sol.n_][sol.p_][sol.r_];
    }

    return out;
}

solution& solution::operator()()
{
    return *this;
}
}

void U10128::operator()() const
{
    int32_t T;
    std::cin >> T;
    solution sol;

    while (T--) {
        std::cin >> sol;
        std::cout << sol << std::endl;
    }
}
