#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u705.h"

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
    U705 instance;
    instance();
}

namespace {
    class solution {
        int32_t cycles_, max_;
        std::vector<std::vector<int32_t>> maze_;
        int32_t rows_, cols_;
        int32_t w_, h_;
    public:
        solution() : cycles_(), max_(), maze_(), rows_(), cols_(), w_(), h_()
        {
            //maze_.reserve(300);
        }

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !(w_ == 0 && h_ == 0);
        }
        solution& operator()();

    private:
        void DFS(int32_t r, int32_t c, int32_t pr, int32_t pc, int32_t tm);
        void forward_slash(int32_t r, int32_t c);
        void backward_slash(int32_t r, int32_t c);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.w_ >> sol.h_;

        if (sol)
        {
            sol.rows_ = sol.h_ * 4;
            sol.cols_ = sol.w_ * 4;
            sol.maze_ = std::vector<std::vector<int32_t>>(sol.rows_, std::vector<int32_t>(sol.cols_, -1));
            std::string line;
            std::getline(in, line);

            for (int32_t i = 0; i < sol.h_; i++)
            {
                std::getline(in, line);

                for (int32_t j = 0; j < sol.w_; j++)
                {
                    if (line[j] == '/')
                    {
                        sol.forward_slash(i * 4, j * 4);
                    }
                    else
                    {
                        sol.backward_slash(i * 4, j * 4);
                    }
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.cycles_ > 0)
        {
            out << sol.cycles_ << " Cycles; the longest has length " << sol.max_ << ".";
        }
        else
        {
            out << "There are no cycles";
        }

        return out;
    }

    solution& solution::operator()()
    {
        cycles_ = max_ = 0;

        for (int i = 0; i < rows_; i++)
        {
            for (int j = 0; j < cols_; j++)
            {
                if (maze_[i][j] == -2)
                {
                    DFS(i, j, -1, -1, 0);
                }
            }
        }

        return *this;
    }

    void solution::DFS(int32_t r, int32_t c, int32_t pr, int32_t pc, int32_t tm)
    {
        static const int32_t mov[][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };
        maze_[r][c] = tm;

        for (int32_t k = 0; k < 8; k++)
        {
            int32_t nr = r + mov[k][0];
            int32_t nc = c + mov[k][1];

            if (nr < 0 || nr >= rows_)
            {
                continue;
            }

            if (nc < 0 || nc >= cols_)
            {
                continue;
            }

            if (nr == pr && nc == pc)
            {
                continue;
            }

            if (maze_[nr][nc] == -1)
            {
                continue;
            }

            if (maze_[nr][nc] == -2)
            {
                DFS(nr, nc, r, c, tm + 1);
            }
            else if (maze_[nr][nc] < tm + 1)
            {
                cycles_++;
                max_ = std::max(max_, (tm + 1 - maze_[nr][nc]) / 2);
            }
        }
    }

    void solution::forward_slash(int32_t r, int32_t c)
    {
        maze_[r + 0][c + 1] = -2;
        maze_[r + 1][c + 0] = -2;
        maze_[r + 2][c + 3] = -2;
        maze_[r + 3][c + 2] = -2;
    }

    void solution::backward_slash(int32_t r, int32_t c)
    {
        maze_[r + 0][c + 2] = -2;
        maze_[r + 1][c + 3] = -2;
        maze_[r + 2][c + 0] = -2;
        maze_[r + 3][c + 1] = -2;
    }
}

void U705::operator()() const
{
    solution sol;
    int32_t caseNo(0);

    while (std::cin >> sol && sol)
    {
        if (caseNo > 0)
        {
            std::cout << std::endl;
        }

        std::cout << "Maze #" << (++caseNo) << std::endl << sol() << std::endl;
    }
}
