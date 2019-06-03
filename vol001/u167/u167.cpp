#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u167.h"

#ifdef _WIN32
    #define _SCL_SECURE_NO_WARNINGS
#endif // WIN32

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <memory>
#include <iomanip>

namespace {
    typedef std::pair<int, int> queen;
    class solver {
        int board_[8][8];
        queen queens_[8];
        int max_;

        void reinit();
        void traverse(int current);
        bool conflict(int qmax, int row, int col);

    public:
        solver() : max_(0)
        {
            reinit();
        }

        friend
        std::istream& operator >> (std::istream& in, solver& s)
        {
            s.reinit();
            std::for_each(s.board_, s.board_ + 8, [&](auto row)
            {
                std::generate_n(row, 8, [&]() -> int { int c; in >> c; return c; });
            });
            return in;
        }

        int solve()
        {
            traverse(0);
            return max_;
        }
    };

    bool solver::conflict(int qmax, int row, int col)
    {
        for (int i = 0; i < qmax; i++)
        {
            if (queens_[i].first == row || queens_[i].second == col)
            {
                return true;
            }

            if (std::abs(queens_[i].first - row) == std::abs(queens_[i].second - col))
            {
                return true;
            }
        }

        return false;
    }

    void solver::reinit()
    {
        max_ = 0;
        std::for_each(board_, board_ + 8, [](auto row)
        {
            std::fill_n(row, 8, 0);
        });
    }

    void solver::traverse(int current)
    {
        if (current == 8)
        {
            int total = 0;

            for (int i = 0; i < 8; i++)
            {
                total += board_[queens_[i].first][queens_[i].second];
            }

            max_ = std::max(total, max_);
            return;
        }

        for (int row = 0; row < 8; row++)
        {
            if (!conflict(current, row, current))
            {
                queens_[current].first = row;
                queens_[current].second = current;
                traverse(current + 1);
            }
        }
    }
}

U167::U167() {}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U167 instance;
    instance();
}
void U167::operator()() const
{
    int N;
    std::cin >> N;
    solver b;

    while (N--)
    {
        std::cin >> b;
        std::cout << std::setw(5) << b.solve() << std::endl;
    }
}
