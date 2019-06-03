#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10024.h"

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

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10024 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), correct_(), g_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        bool correct_;
        std::vector<std::vector<char>> g_;

        bool solve();
        bool check();
        void rotate();
        void upturn();
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        sol.g_.assign(
        {
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0}
        });

        for (size_t i = 0; i < 6; i++)
        {
            for (size_t j = 0; j < 6; j++)
            {
                in >> sol.g_[i][j];
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << (sol.correct_ ? "" : "in") << "correct";
        return out;
    }

    solution_t& solution_t::operator()()
    {
        correct_ = solve();
        return *this;
    }

    bool solution_t::solve()
    {
        for (auto i = 0; i < 2; ++i)
        {
            for (auto j = 0; j < 4; ++j)
            {
                if (check())
                {
                    return true;
                }

                rotate();
            }

            upturn();
        }

        return false;
    }

    bool solution_t::check()
    {
        static char kind[11][4][6] =
        {
            {
                "10000",
                "11110",
                "10000"
            },
            {
                "10000",
                "11110",
                "01000"
            },
            {
                "10000",
                "11110",
                "00100"
            },
            {
                "10000",
                "11110",
                "00010"
            },
            {
                "01000",
                "11110",
                "01000"
            },
            {
                "01000",
                "11110",
                "00100"
            },
            {
                "11000",
                "01110",
                "01000"
            },
            {
                "11000",
                "01110",
                "00100"
            },
            {
                "11000",
                "01110",
                "00010"
            },
            {
                "11100",
                "00111",
                "00000"
            },
            {
                "11000",
                "01100",
                "00110"
            },
        };

        for (auto i = 0; i < 6; i++)
        {
            for (auto j = 0; j < 6; j++)
            {
                for (auto k = 0; k < 11; k++)
                {
                    auto p = 0;

                    for (; p < 3; p++)
                        for (auto q = 0; q < 5; q++)
                        {
                            if (kind[k][p][q] == '0')
                            {
                                continue;
                            }

                            if (i + p >= 6 || j + q >= 6 || g_[i + p][j + q] != kind[k][p][q])
                            {
                                p = 10, q = 10;
                            }
                        }

                    if (p == 3)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void solution_t::rotate()
    {
        char temp[6][6];

        for (auto i = 0; i < 6; i++)
            for (auto j = 0; j < 6; j++)
            {
                temp[i][j] = g_[5 - j][i];
            }

        for (auto i = 0; i < 6; i++)
            for (auto j = 0; j < 6; j++)
            {
                g_[i][j] = temp[i][j];
            }
    }

    void solution_t::upturn()
    {
        char temp[6][6];

        for (auto i = 0; i < 6; i++)
            for (auto j = 0; j < 6; j++)
            {
                temp[i][j] = g_[i][5 - j];
            }

        for (auto i = 0; i < 6; i++)
            for (auto j = 0; j < 6; j++)
            {
                g_[i][j] = temp[i][j];
            }
    }
}

void U10024::operator()() const
{
    solution_t sol;

    std::string sep;

    while (std::cin >> sol && sol)
    {
        std::cout << sep << sol() << std::endl;
        sep = "\n";
    }
}
