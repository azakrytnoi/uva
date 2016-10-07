#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u116.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

namespace
{
typedef std::vector<std::vector<int>> matrix;
class solver
{
    int N_, M_;
    int best_;
    matrix matrix_;
    matrix costs_;
    matrix path_;
    std::vector<int> trace_;

    int dp(int row, int col);

public:

    solver() : N_(0), M_(0), best_(0), matrix_(), costs_(), path_(), trace_()
    {
        matrix_.resize(10);
        costs_.resize(10);
        path_.resize(10);
        trace_.reserve(100);
        for (int i = 0; i < 10; i++) {
            matrix_[i].reserve(100);
            costs_[i].reserve(100);
            path_[i].reserve(100);
        }
    }

    friend
    std::istream& operator >> (std::istream& in, solver& s)
    {
        if (in >> s.M_ >> s.N_) {
            for (int i = 0; i < s.M_; i++) {
                s.matrix_[i].clear();
                std::generate_n(std::back_inserter(s.matrix_[i]), s.N_, [&]() -> int {
                    int tmp; std::cin >> tmp;
                    return tmp; });
                s.costs_[i].clear();
                std::fill_n(std::back_inserter(s.costs_[i]), s.N_, std::numeric_limits<int>::max());
                s.path_[i].clear();
                std::fill_n(std::back_inserter(s.path_[i]), s.N_, 0);
            }
            s.trace_.clear();
            s.best_ = std::numeric_limits<int>::max();
        }
        return in;
    }

    friend
    std::ostream& operator << (std::ostream& out, const solver& s)
    {
        std::ostream_iterator<int> oit(out, " ");
        std::copy(s.trace_.begin(), s.trace_.end(), oit);
        out << std::endl << s.best_;
        return out;
    }

    void solve();
};

void solver::solve()
{
    /*
     * DP problem, relation is the minimum cost length to current cell from
     * cell to left and cell to above left.
     */
    int row(0);
    for (int i = 0; i < M_; i++) {
        dp(i, 0);
        if (costs_[i][0] < best_) {
            best_ = costs_[i][0];
            row = i;
        }
    }
    // Print path.
    for (int j = 0; j < N_; j++) {
        trace_.push_back(row + 1);
        row = path_[row][j];
    }
}

int solver::dp(int row, int col)
{
    if (col >= N_) {
        return 0;
    }
    if (costs_[row][col] != std::numeric_limits<int>::max()) {
        return costs_[row][col];
    }

    int next[3] = { row - 1, row, row + 1 };
    if (row == 0) {
        next[0] = M_ - 1;
    }
    if (row == M_ - 1) {
        next[2] = 0;
    }

    for (int k = 0; k < 3; k++) {
        int val = matrix_[row][col] + dp(next[k], col + 1);
        // Second argument is to preserve lexicographical output.
        if (costs_[row][col] > val || (costs_[row][col] == val && path_[row][col] > next[k])) {
            costs_[row][col] = val;
            // Store direction when cost matrix is updated.
            path_[row][col] = next[k];
        }
    }
    return costs_[row][col];
}
}

U116::U116() {}

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U116 instance;
    instance();
}
void U116::operator()() const
{
    solver s;
    while (std::cin >> s) {
        s.solve();
        std::cout << s << std::endl;
    }
}
