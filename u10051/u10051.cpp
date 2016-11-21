#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10051.h"

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
    U10051 instance;
    instance();
}

namespace {
    std::string face[6] = { "front", "back", "left", "right", "top", "bottom" };

    class solution {
        int32_t n_;
        int32_t max_, pathIdx_;
        int32_t idx_;
        std::vector<int32_t> num_, top_, bot_, df_, path_;
    public:
        solution() : n_(), max_(), pathIdx_(), idx_(), num_(), top_(), bot_(), df_(), path_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return n_ > 0;
        }
        solution& operator()();

    private:
        void printPath(std::ostream& out, int32_t current) const;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_;

        if (sol) {
            sol.num_.resize(sol.n_ * 6);
            sol.top_.resize(sol.n_ * 6);
            sol.bot_.resize(sol.n_ * 6);
            sol.df_.resize(sol.n_ * 6);
            sol.idx_ = 0;

            for (int32_t i = 0; i < sol.n_; i++) {
                std::vector<int32_t> cube;
                cube.reserve(6);
                std::generate_n(std::back_inserter(cube), 6, [&]() {
                    int32_t tmp;
                    in >> tmp;
                    return tmp;
                });

                for (int32_t j = 0; j < 6; j++, sol.idx_++) {
                    sol.df_[sol.idx_] = j;
                    sol.num_[sol.idx_] = i + 1;
                    sol.top_[sol.idx_] = cube[j];

                    if (j % 2 == 0) {
                        sol.bot_[sol.idx_] = cube[j + 1];
                    } else {
                        sol.bot_[sol.idx_] = cube[j - 1];
                    }
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << (sol.max_ + 1);
        sol.printPath(out, sol.pathIdx_);
        return out;
    }

    solution& solution::operator()()
    {
        path_.swap(std::vector<int32_t>(n_ * 6, -1));
        std::vector<int32_t> dp(n_ * 6);

        for (int32_t i = 0; i < idx_; i++) {
            for (int32_t j = 0; j < idx_; j++) {
                if (num_[j] > num_[i] && bot_[i] == top_[j] && dp[j] < dp[i] + 1) {
                    dp[j] = dp[i] + 1;
                    path_[j] = i;
                }
            }
        }

        max_ = pathIdx_ = 0;

        for (int32_t i = 0; i < idx_; i++) {
            if (dp[i] > max_) {
                max_ = dp[i];
                pathIdx_ = i;
            }
        }

        return *this;
    }

    void solution::printPath(std::ostream& out, int32_t current) const
    {
        if (current == -1) {
            return;
        }

        printPath(out, path_[current]);
        out << std::endl << num_[current] << ' ' << face[df_[current]];
    }
}

void U10051::operator()() const
{
    solution sol;
    int32_t caseNo(0);

    while (std::cin >> sol && sol) {
        if (caseNo > 0) {
            std::cout << std::endl;
        }

        std::cout << "Case #" << (++caseNo) << std::endl << sol() << std::endl;
    }
}
