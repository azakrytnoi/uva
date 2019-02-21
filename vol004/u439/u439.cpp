#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u439.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <array>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U439 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : moves_(), positions_()
        {
            auto is_knight_move = [](int32_t x1, int32_t y1, int32_t x2, int32_t y2) -> bool {
                auto dx = std::abs(x1 - x2), dy = std::abs(y1 - y2);
                return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
            };

            for (auto i = 0; i < DIMENSION; ++i) {
                for (auto j = 0; j < DIMENSION; ++j) {
                    if (i == j) {
                        moves_[i][j] = 0;
                    } else if (is_knight_move(i / SIZE, i % SIZE, j / SIZE, j % SIZE)) {
                        moves_[i][j] = 1;
                    } else {
                        moves_[i][j] = INF;
                    }
                }
            }

            for (auto k = 0; k < DIMENSION; ++k) {
                for (auto i = 0; i < DIMENSION; ++i) {
                    for (auto j = 0; j < DIMENSION; ++j) {
                        moves_[i][j] = std::min(moves_[i][j], moves_[i][k] + moves_[k][j]);
                    }
                }
            }
        }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        static const int32_t SIZE = 8;
        static const int32_t DIMENSION = SIZE * SIZE;
        std::array<std::array<uint32_t, DIMENSION>, DIMENSION> moves_;
        std::pair<std::string, std::string> positions_;
        static const int32_t INF = 100000;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.positions_.first >> sol.positions_.second;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        auto startPos = (sol.positions_.first[0] - 'a') * sol.SIZE + (sol.positions_.first[1] - '1');
        auto endPos = (sol.positions_.second[0] - 'a') * sol.SIZE + (sol.positions_.second[1] - '1');

        out << "To get from " << sol.positions_.first << " to " << sol.positions_.second << " takes " << sol.moves_[startPos][endPos] <<
            " knight moves.";
        return out;
    }

    solution_t& solution_t::operator()()
    {
        return *this;
    }

}

void U439::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
