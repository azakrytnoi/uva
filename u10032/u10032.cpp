#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10032.h"

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
    U10032 instance;
    instance();
}

namespace {
class solution {
    std::vector<int32_t> weights_;
    std::pair<int32_t, int32_t> teams_;
    int32_t total_;
public:
    solution() : weights_(), teams_(), total_() {}

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
    int32_t n;

    if (in >> n) {
        sol.weights_.clear();
        sol.weights_.reserve(n);
        sol.total_ = 0;
        std::generate_n(std::back_inserter(sol.weights_), n, [&]() {
            int32_t tmp;
            in >> tmp;
            sol.total_ += tmp;
            return tmp;
        });
        sol.teams_ = { 0, 0 };
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.teams_.first << ' ' << sol.teams_.second;
    return out;
}

solution& solution::operator()()
{
    std::vector<int64_t> dp(weights_.size() * 450);
    dp[0] = 1;

    for (size_t i = 0; i < weights_.size(); i++) {
        for (int j = total_; j > -1; j--) {
            if (dp[j]) {
                dp[j + weights_[i]] |= (dp[j] << 1);
            }
        }
    }

    int32_t half(total_ / 2);
    int64_t bit(1ll << (weights_.size() / 2));

    int32_t median(0);

    for (int i = half, j = half; i > -1 && j <= total_; i--, j++) {
        if (dp[i] & bit) {
            median = i;
            break;
        }

        if (dp[j] & bit) {
            median = j;
            break;
        }
    }

    teams_ = { std::min(median, total_ - median), std::max(median, total_ - median) };

    return *this;
}
}

void U10032::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;
    bool next(false);

    while (N--) {
        if (next) {
            std::cout << std::endl;
        }

        next = true;
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
