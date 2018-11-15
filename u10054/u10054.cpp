#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10054.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10054 instance;
    instance();
}

namespace {

class solution {
    std::vector<size_t> answers_;
    constexpr static size_t MAX_N = 100;
    int16_t g_[MAX_N][MAX_N];
    size_t start_;
    bool solved_;
public:
    solution() : answers_(), start_(), solved_()
    {
        answers_.reserve(MAX_N);
    }

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return true;
    }
    solution& operator()();

private:
    void dfs(size_t current);
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.solved_ = false;
    std::memset(sol.g_, 0, sizeof(sol.g_));
    sol.answers_.clear();
    size_t n;
    in >> n;

    for (size_t i = 0; i < n; i++) {
        size_t temp1, temp2;
        in >> temp1 >> temp2;
        sol.g_[temp1][temp2] += 1;
        sol.g_[temp2][temp1] += 1;

        if (i == 0) {
            sol.start_ = temp1;
        }
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    if (sol.solved_) {
        for (size_t i = 0; i < sol.answers_.size() - 1; i++) {
            out << sol.answers_[i] << " " << sol.answers_[i + 1] << std::endl;
        }
    } else {
        out << "some beads may be lost" << std::endl;
    }

    return out;
}

solution& solution::operator()()
{
    for (size_t i = 0; i < MAX_N; i++) {
        size_t check(0);

        for (size_t j = 0; j < MAX_N; j++) {
            check += g_[i][j];
        }

        if (check % 2 != 0) {
            return *this;
        }
    }

    dfs(start_);
    solved_ = true;
    return *this;
}

void solution::dfs(size_t current)
{
    for (size_t i = 0; i < MAX_N; i++) {
        if (g_[current][i] > 0) {
            g_[current][i]--;
            g_[i][current]--;
            dfs(i);
        }
    }

    answers_.push_back(current);
}

}

void U10054::operator()() const
{
    size_t N, caseNo(0);
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << "Case #" << (++caseNo) << std::endl << sol() << std::endl;
    }
}
