#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10181.h"

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
    U10181 instance;
    instance();
}

namespace {
    constexpr int SIZE = 4;
    constexpr int N = SIZE * SIZE;
    constexpr int DIR_N = 4;
    constexpr int INF = 0x7fffffff;
    constexpr int MAX_DEPTH = 1000;

    class solution {
        std::vector<int8_t> goal_;
        std::vector<int8_t> cur_;
        std::vector<int8_t> start_;
        std::vector<int8_t> path_;
        int bound_;
        bool solved_;
        static const int8_t md_[];
        static const int16_t dir_[][2];

    public:
        solution() : goal_(N), cur_(N), start_(N), path_(MAX_DEPTH), bound_(), solved_()
        {
            for (int i = 0; i < N - 1; ++i)
            {
                goal_[i] = i + 1;
            }

            goal_[N - 1] = 0;
        }

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return false;
        }
        solution& operator()();

    private:
        static bool inv_judge(std::vector<int8_t>& s);
        int heu() const;
        int dfs(int pos, int depth, int8_t d);
        int IDAstar();
    };

    const int8_t solution::md_[] = { 'U', 'L', 'R', 'D' };
    const int16_t solution::dir_[][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };

    int solution::IDAstar()
    {
        solved_ = false;
        bound_ = heu();

        int i(0);

        for (; i < N && start_[i]; ++i);

        while (!solved_ && bound_ < 100)
        {
            bound_ = dfs(i, 0, -10);
        }

        return bound_;
    }

    int solution::dfs(int pos, int depth, int8_t d)
    {
        int h = heu();

        if (depth + h > bound_)
        {
            return depth + h;
        }

        if (h == 0)
        {
            solved_ = true;
            return depth;
        }

        int nbound = INF;

        for (int i = 0; i < DIR_N; ++i)
        {
            if (i + d == DIR_N - 1)
            {
                continue;
            }

            int nx = pos / SIZE + dir_[i][0];
            int ny = pos % SIZE + dir_[i][1];

            if (0 <= nx && nx < SIZE && 0 <= ny && ny < SIZE)
            {
                path_[depth] = i;
                int npos = nx * SIZE + ny;
                cur_[pos] = cur_[npos];
                cur_[npos] = 0;     /* pos -> npos */
                int t = dfs(npos, depth + 1, i);

                if (solved_)
                {
                    return t;
                }

                nbound = std::min(nbound, t);
                cur_[npos] = cur_[pos];
                cur_[pos] = 0;
            }
        }

        return nbound;
    }

    int solution::heu() const
    {
        int ret(0);

        for (int i = 0; i < N; ++i)
        {
            int8_t ch = goal_[i];

            if (ch == 0)
            {
                continue;
            }

            for (int j = 0; j < N; ++j)
            {
                if (ch == cur_[j])
                {
                    ret = ret + std::abs(i / SIZE - j / SIZE) + std::abs(i % SIZE - j % SIZE);
                }
            }
        }

        return ret;
    }

    bool solution::inv_judge(std::vector<int8_t>& s)
    {
        int8_t ret(0);

        for (int i = 0; i < N - 1; ++i)
        {
            int8_t ch;

            if ((ch = s[i]) == 0 && ((i / SIZE + 1) & 0x1))
            {
                ret = 1 - ret;
            }

            for (int j = i + 1; j < N; ++j)
            {
                if (s[j] && s[j] < ch)
                {
                    ret = 1 - ret;
                }
            }
        }

        return ret == 0;
    }

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.start_.clear();
        std::generate_n(std::back_inserter(sol.start_), N, [&]()
        {
            int tmp;
            in >> tmp;
            return tmp;
        });
        sol.cur_.assign(sol.start_.begin(), sol.start_.end());
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (!sol.solved_)
        {
            out << "This puzzle is not solvable.";
        }
        else
        {
            for (auto it = sol.path_.begin(); it != sol.path_.end() && *it != -1; ++it)
            {
                out << sol.md_[*it];
            }
        }

        return out;
    }

    solution& solution::operator()()
    {
        solved_ = inv_judge(start_);

        if (solved_)
        {
            path_.clear();
            std::generate_n(std::back_inserter(path_), MAX_DEPTH, []()
            {
                return -1;
            });
            IDAstar();
        }

        return *this;
    }
}

void U10181::operator()() const
{
    int nCases;
    std::cin >> nCases;
    solution sol;

    while (nCases--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
