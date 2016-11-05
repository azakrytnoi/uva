#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10270.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cstring>

#include "gcd.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10270 instance;
    instance();
}

namespace {
    constexpr int32_t NMAX = 20;
    constexpr int32_t NPRIME = 11;
    constexpr int32_t SMAX = 1024;
    constexpr int32_t PMAX = 2500;
    constexpr int32_t NCELL = 50;

    struct point {
        int32_t x_, y_;
    };
    struct square {
        point topLeft_;
        int32_t size_;
    };
    int32_t tip[24] = { 6, 8, 9, 6, 11, 11, 6, 12, 13, 6, 13, 8, 6, 14, 15, 6, 8, 15, 6,
                        15, 16, 6, 16, 9
                      };
    square squares[NMAX];
    square best[NMAX];

    int trick[NPRIME][NMAX] = {
        {11, 11, 6, 5, 5, 4, 2, 2, 2, 2, 1, 1, 1},              // 11
        {13, 11, 7, 6, 6, 4, 3, 3, 2, 2, 2, 1, 1},              // 13
        {17, 12, 9, 8, 8, 5, 4, 4, 3, 2, 2, 2, 1, 1},               // 17
        {19, 13, 10, 9, 9, 5, 5, 5, 3, 2, 2, 2, 1, 1, 1},           // 19
        {23, 13, 12, 11, 11, 7, 5, 5, 4, 3, 3, 2, 2, 1, 1},         // 23
        {29, 14, 17, 12, 12, 9, 8, 8, 4, 4, 3, 2, 2, 2, 1, 1},          // 29
        {31, 15, 16, 15, 15, 8, 8, 8, 4, 4, 4, 2, 2, 2, 1, 1, 1},       // 31
        {37, 15, 19, 18, 18, 11, 8, 8, 6, 5, 5, 3, 3, 2, 1, 1, 1},      // 37
        {41, 15, 23, 18, 18, 12, 11, 11, 7, 5, 4, 3, 3, 2, 2, 1, 1},        // 41
        {43, 16, 22, 21, 21, 11, 11, 11, 6, 5, 5, 3, 3, 3, 2, 1, 1, 1},     // 43
        {47, 16, 24, 23, 23, 12, 12, 12, 7, 5, 5, 4, 3, 3, 2, 2, 1, 1},         // 47
    };

    int smallest;
    int ncount[NCELL];
    int cell[NCELL][NCELL];
    int backup[NCELL][NCELL];
    int ncache[NMAX];
    int cache[NMAX][SMAX][NMAX];

    bool found;
    bool finished;

    class solution {
        int32_t n_;
    public:
        solution() : n_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        static void building(int x, int y, int size, std::ostream& out)
        {
            out << x << " " << y << " " << size << std::endl;
        }
        void solve_it_by_hard_work();
        void solve_it_by_tip();
        void solve_it_by_trick();

        void fill(int blocks[], int ncurrent, int goal, bool display_when_find);
        int find(int size, point points[PMAX]);
        void cut_by_hard_work(int area, int blocks[NMAX], int nblocks);
        void cut_by_tip(int area, int blocks[NMAX], int nblocks, int goal);

        void print(square s[NMAX], int nsquares);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.n_ % 2 == 0) {
            int size = sol.n_ / 2;

            out << "4" << std::endl;

            solution::building(1, 1, size, out);
            solution::building(1, 1 + size, size, out);
            solution::building(1 + size, 1, size, out);
            solution::building(1 + size, 1 + size, size, out);
        } else if (sol.n_ % 6 == 3) {
            int size = sol.n_ / 3;

            out << "6" << std::endl;

            solution::building(1, 1, size * 2, out);
            solution::building(1 + size * 2, 1, size, out);
            solution::building(1 + size * 2, 1 + size, size, out);
            solution::building(1 + size * 2, 1 + size * 2, size, out);
            solution::building(1, 1 + size * 2, size, out);
            solution::building(1 + size, 1 + size * 2, size, out);
        } else if (sol.n_ % 5 == 0) {
            int size = sol.n_ / 5;

            out << "8" << std::endl;

            solution::building(1, 1, size * 3, out);
            solution::building(1 + size * 3, 1, size * 2, out);
            solution::building(1 + size * 3, 1 + size * 2, size * 2, out);
            solution::building(1, 1 + size * 3, size * 2, out);
            solution::building(1 + size * 2, 1 + size * 3, size, out);
            solution::building(1 + size * 2, 1 + size * 4, size, out);
            solution::building(1 + size * 3, 1 + size * 4, size, out);
            solution::building(1 + size * 4, 1 + size * 4, size, out);
        } else if (sol.n_ % 7 == 0) {
            int size = sol.n_ / 7;

            out << "9" << std::endl;

            solution::building(1, 1, size * 4, out);
            solution::building(1 + size * 4, 1, size * 3, out);
            solution::building(1, 1 + size * 4, size * 3, out);
            solution::building(1 + size * 3, 1 + size * 5, size * 2, out);
            solution::building(1 + size * 5, 1 + size * 5, size * 2, out);
            solution::building(1 + size * 5, 1 + size * 3, size * 2, out);
            solution::building(1 + size * 4, 1 + size * 3, size, out);
            solution::building(1 + size * 4, 1 + size * 4, size, out);
            solution::building(1 + size * 3, 1 + size * 4, size, out);
        }

        return out;
    }

    solution& solution::operator()()
    {
        if (!((n_ % 2 == 0) || (n_ % 6 == 3) || (n_ % 5 == 0) || (n_ % 7 == 0))) {
            solve_it_by_trick();
        }

        return *this;
    }

    void solution::solve_it_by_trick()
    {
        int blocks[NMAX];
        int nblocks;

        for (int r = 0; r < NPRIME; r++)
            if (trick[r][0] == n_) {
                nblocks = trick[r][1];

                for (int c = 0; c < nblocks; c++) {
                    blocks[c] = trick[r][c + 2];
                }

                break;
            }

        finished = false;

        memset(cell, 0, sizeof(cell));

        fill(blocks, 0, nblocks, true);
    }

    void solution::solve_it_by_tip()
    {
        int blocks[NMAX];

        finished = false;

        memset(ncache, 0, sizeof(ncache));

        int goal = tip[n_ / 2 - 1];

        cut_by_tip(n_ * n_, blocks, 0, goal);
    }

    void solution::solve_it_by_hard_work()
    {
        int current[NMAX];

        memset(ncache, 0, sizeof(ncache));

        smallest = 1 + n_ / 2 + (n_ - 2) / 2 + 4;
        int threshold = smallest;

        for (int s = (n_ / 2 + 1); s < (n_ - 2); s++) {
            smallest = 2;

            math::gcd(s, n_ - s);

            if (threshold > smallest) {
                threshold = smallest;
            }
        }

        smallest = threshold;

        cut_by_hard_work(n_ * n_, current, 0);

        print(best, smallest);
    }

    void solution::fill(int blocks[], int ncurrent, int goal, bool display_when_find)
    {
        if (ncurrent == goal) {
            memcpy(backup, cell, sizeof(cell));

            if (display_when_find) {
                print(squares, ncurrent);
            } else {
                memcpy(best, squares, sizeof(squares));
            }

            finished = true;
        } else {
            int npoints;
            point points[PMAX];

            if ((npoints = find(blocks[ncurrent], points)) == 0) {
                return;
            }

            for (int i = 0; i < npoints; i++) {
                int x = points[i].x_;
                int y = points[i].y_;
                int s = blocks[ncurrent];

                if (ncurrent == 0 && (y != 0 || x != 0)) {
                    continue;
                }

                if (ncurrent == 1 && (y != 0 || x != blocks[0])) {
                    continue;
                }

                if (ncurrent == 2 && (y != blocks[0] || x != 0)) {
                    continue;
                }

                if (ncurrent == 3 && x != (n_ - blocks[ncurrent])) {
                    continue;
                }

                if (ncurrent == 4 && (x != (n_ - blocks[ncurrent]) &&
                                      y != (n_ - blocks[ncurrent]))) {
                    continue;
                }

                squares[ncurrent].topLeft_.x_ = (x + 1);
                squares[ncurrent].topLeft_.y_ = (y + 1);
                squares[ncurrent].size_ = s;

                for (int gy = y; gy < (y + s); gy++)
                    for (int gx = x; gx < (x + s); gx++) {
                        cell[gy][gx] = s;
                    }

                fill(blocks, ncurrent + 1, goal, display_when_find);

                if (finished) {
                    return;
                }

                for (int gy = y; gy < (y + s); gy++)
                    for (int gx = x; gx < (x + s); gx++) {
                        cell[gy][gx] = 0;
                    }
            }
        }
    }

    int solution::find(int size, point points[PMAX])
    {
        int npoints = 0;

        for (int y = 0; y <= (n_ - size); y++)
            for (int x = 0; x <= (n_ - size); x++)
                if (cell[y][x] == 0) {
                    bool empty = true;

                    for (int i = y; i < (y + size); i++) {
                        for (int j = x; j < (x + size); j++)
                            if (cell[i][j] != 0) {
                                empty = false;
                                break;
                            }

                        if (!empty) {
                            break;
                        }
                    }

                    if (empty) {
                        points[npoints].x_ = x;
                        points[npoints].y_ = y;

                        npoints++;
                    }
                }

        return npoints;
    }

    void solution::cut_by_hard_work(int area, int blocks[NMAX], int nblocks)
    {
        if (area >= 0 && nblocks > smallest) {
            return;
        }

        if (area == 0 && nblocks == smallest && found) {
            return;
        }

        if (area == 0 && ncount[1] <= 1) {
            return;
        }

        if (area == 0) {
            int temp[NMAX];

            memcpy(temp, blocks, NMAX * sizeof(int));

            std::sort(temp, temp + nblocks, [](int x, int y) {
                return x > y;
            });

            memset(cell, 0, sizeof(cell));

            finished = false;
            fill(temp, 0, nblocks, false);

            if (finished) {
                smallest = nblocks;
                found = true;
            }
        } else {
            int32_t r;

            for (r = n_ - 2; r >= 1; r--)
                if (area >= (r * r)) {
                    break;
                }

            int32_t c = r;
            int32_t step = (nblocks == 0) ? 1 : (-1);
            r = (nblocks == 0) ? 1 : r;

            for (; c >= 1; c--, r += step) {
                if (nblocks == 0 && r < (n_ / 2 + 1)) {
                    continue;
                }

                if (nblocks == 1 && (r + blocks[0]) != n_) {
                    continue;
                }

                if (nblocks == 2 && r != blocks[1]) {
                    continue;
                }

                if (nblocks == 4 && (r + blocks[3]) != blocks[0]) {
                    continue;
                }

                if ((ncount[r] + 1) > 4) {
                    continue;
                }

                ncount[r]++;

                blocks[nblocks] = r;

                cut_by_hard_work(area - (r * r), blocks, nblocks + 1);

                ncount[r]--;
            }
        }
    }

    void solution::cut_by_tip(int area, int blocks[NMAX], int nblocks, int goal)
    {
        if (area > 0 && nblocks == goal) {
            return;
        }

        if (area == 0 && nblocks != goal) {
            return;
        }

        if (area == 0) {
            int temp[NMAX];

            memcpy(temp, blocks, NMAX * sizeof(int));

            std::sort(temp, temp + nblocks, [](int x, int y) {
                return x > y;
            });

            bool exist = false;

            for (int i = 0; i < ncache[nblocks - 1]; i++) {
                bool equal = true;

                for (int j = 0; j < nblocks; j++)
                    if (cache[nblocks - 1][i][j] != temp[j]) {
                        equal = false;
                        break;
                    }

                if (equal) {
                    exist = true;
                    break;
                }
            }

            if (!exist) {
                memset(cell, 0, sizeof(cell));

                fill(temp, 0, nblocks, true);

                if (finished) {
                    return;
                }

                memcpy(cache[nblocks - 1][ncache[nblocks - 1]++],
                       temp, sizeof(temp));
            }

        } else {
            int up;

            for (int u = n_ - 1; u >= 1; u--)
                if (area >= (u * u)) {
                    up = u;
                    break;
                }

            for (int r = (up / 2 + 1); r <= up; r++) {
                if (nblocks == 1 && (r + blocks[0]) != n_) {
                    continue;
                }

                if (nblocks == 2 && r != blocks[1]) {
                    continue;
                }

                if (nblocks == 4 && (r + blocks[3]) != blocks[0]) {
                    continue;
                }

                blocks[nblocks] = r;

                cut_by_tip(area - (r * r), blocks, nblocks + 1, goal);

                if (finished) {
                    return;
                }
            }
        }
    }

    void solution::print(square s[NMAX], int nsquares)
    {
        std::cout << nsquares << std::endl;

        for (int i = 0; i < nsquares; i++) {
            std::cout << s[i].topLeft_.x_ << " " << s[i].topLeft_.y_ << " " << s[i].size_ << std::endl;
        }
    }

}

void U10270::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}



