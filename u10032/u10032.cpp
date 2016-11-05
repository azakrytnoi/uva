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

namespace
{

class solution
{
    constexpr static int32_t maxW_ = 450;
    std::vector<int32_t> weights_;
    int32_t team1_, team2_;
public:
    solution() : weights_(), team1_(), team2_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const { return true; }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    int32_t n;
    if (in >> n)
    {
        sol.weights_.clear();
        sol.weights_.reserve(n);
        std::generate_n(std::back_inserter(sol.weights_), n, [&](){
            int32_t tmp;
            in >> tmp;
            return tmp;
        });
    }
  return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.team1_ << ' ' << sol.team2_;
  return out;
}

solution& solution::operator()()
{
    team1_ = team2_ = 0;
    int32_t summ = std::accumulate(weights_.begin(), weights_.end(), 0);

    std::vector<int64_t> dp (summ + 1);

    dp[0] = 1;
    std::for_each(weights_.begin(), weights_.end(), [&](auto w) {
        for (int32_t dpIt = summ; dpIt >= 0; --dpIt) {
            if (dp[dpIt]) {
                dp[dpIt + w] |= dp[dpIt] << 1;
            }
        }
    });

    int32_t minDiff = std::numeric_limits<int32_t>::max();
    int64_t n = 1 << (weights_.size() / 2);
    for(int32_t i = 0; i <= summ; ++i) {
        if (n & dp[i]) {
            int32_t diff = std::abs(i - (summ - i));
            if (diff < minDiff) {
                minDiff = diff;
                team1_ = std::min(i, summ - i);
                team2_ = std::max(i, summ - i);
            }
        }
    }
  return *this;
}

}

void U10032::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;
    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl << std::endl;
    }
}
