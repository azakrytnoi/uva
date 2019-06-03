#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10081.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10081 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : k_(), n_(), res_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        size_t k_, n_;
        double_t res_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.k_ >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << std::fixed << std::setprecision(5) << sol.res_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<std::vector<double_t>> dp(n_ + 1, std::vector <double_t>(k_ + 1, 0.0));
        std::fill(dp[1].begin(), dp[1].end(), 1.0);

        for (auto i = 2u; i <= n_; ++i)
        {
            dp[i][0] = dp[i - 1][0] + dp[i - 1][1];
            dp[i][k_] = dp[i - 1][k_] + dp[i - 1][k_ - 1];

            for (auto j = 1u; j < k_; j++)
            {
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j] + dp[i - 1][j + 1];
            }
        }

        res_ = std::accumulate(dp[n_].begin(), dp[n_].end(), 0.0);
        auto total (std::pow(k_ + 1, n_));
        res_ = (res_ * 100.0) / total;
        return *this;
    }

}

void U10081::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
