#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u861.h"

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
    U861 instance;
    instance();
}

namespace {
    class solution {
        int32_t n_, k_;
        int32_t result_;
    public:
        solution() : n_(), k_(), result_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !(n_ == 0 && k_ == 0);
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_ >> sol.k_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.result_;
        return out;
    }

    solution& solution::operator()()
    {
        if (n_ == 1 || k_ == 0)
        {
            result_ = 1;

        }
        else
        {
            int32_t row1[20] = {}, row2[20] = {};

            for (int32_t i = 0; i < n_; i++)
            {
                for (int32_t j = 0; j < n_; j++)
                {
                    if ((i + j) % 2)
                    {
                        row1[(i + j) / 2]++;

                    }
                    else
                    {
                        row2[(i + j) / 2]++;
                    }
                }
            }

            std::sort(row1, row1 + n_ - 1);
            std::sort(row2, row2 + n_);
            int32_t dp1[20][20] = {}, dp2[20][20] = {};
            dp1[0][0] = 1, dp1[0][1] = row1[0];
            dp2[0][0] = 1, dp2[0][1] = row2[0];

            for (int32_t i = 1; i < n_ - 1; i++)
            {
                dp1[i][0] = 1;

                for (int32_t j = 1; j <= row1[i]; j++)
                {
                    dp1[i][j] = dp1[i - 1][j] + dp1[i - 1][j - 1] * (row1[i] - (j - 1));
                }
            }

            for (int32_t i = 1; i < n_; i++)
            {
                dp2[i][0] = 1;

                for (int32_t j = 1; j <= row2[i]; j++)
                {
                    dp2[i][j] = dp2[i - 1][j] + dp2[i - 1][j - 1] * (row2[i] - (j - 1));
                }
            }

            result_ = 0;

            for (int32_t i = 0; i <= k_; i++)
            {
                result_ += dp1[n_ - 2][i] * dp2[n_ - 1][k_ - i];
            }
        }

        return *this;
    }
}

void U861::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}