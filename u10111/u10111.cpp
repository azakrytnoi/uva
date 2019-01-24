#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10111.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <array>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10111 instance;
    instance();
}

namespace {

    enum class cell_t : char {
        Nobody = '.', x = 'x', o = 'o'
    };

//    bool operator < (const cell_t lhs, const cell_t rhs) {
//
//    }

    class board_t {
    public:
        constexpr static size_t size = 4;
        std::array<std::array<cell_t, size>, size> grid_;
        bool x_turn_;

        board_t() : grid_(), x_turn_() {}

        bool operator < (const board_t& rhs) const
        {
            for (size_t i = 0; i < size; i++) {
                for (size_t j = 0; j < size; j++) {
                    if (grid_[i][j] != rhs.grid_[i][j]) {
                        return grid_[i][j] < rhs.grid_[i][j];
                    }
                }
            }

            return false;
        }
    };

    class solution {
    public:
        solution() : board_(), win_(), done_(), o_lose_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return not done_;
        }
        solution& operator()();

    private:
        board_t board_;
        std::pair<int64_t, int64_t> win_;
        bool done_;
        std::map<board_t, std::pair<size_t, size_t>> o_lose_;

        cell_t winner();
        std::pair<int64_t, int64_t> check_o_lose();
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        char ch;
        in >> ch;

        if (not (sol.done_ = (ch == '$'))) {
            for (size_t i = 0; i < board_t::size; i++) {
                for (size_t j = 0; j < board_t::size; j++) {
                    in >> ch;
                    sol.board_.grid_[i][j] = static_cast<cell_t>(ch);
                }
            }

            sol.board_.x_turn_ = true;
            sol.win_ = {0, 0};
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.win_.first != -1 && sol.win_.second != -1) {
            out << "(" << sol.win_.first << "," << sol.win_.second << ")";
        } else {
            out << "#####";
        }

        return out;
    }

    solution& solution::operator()()
    {
        win_ = check_o_lose();
        return *this;
    }

    cell_t solution::winner()
    {
        for (size_t row = 0; row < board_t::size; row++) {
            if (board_.grid_[row][0] == cell_t::Nobody || board_.grid_[0][row] == cell_t::Nobody) {
                continue;
            }

            bool same_row(true), same_col(true);

            for (size_t col = 1; col < board_t::size; col++) {
                if (board_.grid_[row][col] != board_.grid_[row][0]) {
                    same_row = false;
                }

                if (board_.grid_[col][row] != board_.grid_[0][row]) {
                    same_col = false;
                }
            }

            if (same_row) {
                return board_.grid_[row][0];
            }

            if (same_col) {
                return board_.grid_[0][row];
            }
        }

        if (board_.grid_[0][0] != cell_t::Nobody && board_.grid_[0][0] == board_.grid_[1][1] && board_.grid_[0][0] == board_.grid_[2][2] &&
                board_.grid_[0][0] == board_.grid_[3][3]) {
            return board_.grid_[0][0];
        }

        if (board_.grid_[0][3] != cell_t::Nobody && board_.grid_[0][3] == board_.grid_[1][2] && board_.grid_[0][3] == board_.grid_[2][1] &&
                board_.grid_[0][3] == board_.grid_[3][0]) {
            return board_.grid_[0][3];
        }

        return cell_t::Nobody;
    }

    std::pair<int64_t, int64_t> solution::check_o_lose()
    {
        auto checked_board = o_lose_.find(board_);

        if (checked_board == o_lose_.end()) {
            std::pair<int64_t, int64_t> current = {-1, -1};
            auto win = winner();

            if (win != cell_t::Nobody) {
                current = (win == cell_t::x ? std::make_pair(0l, 0l) : current);
            } else {
                bool all_checked(true);

                for (size_t col = 0; col < board_t::size; col++) {
                    for (size_t row = 0; row < board_t::size; row++) {
                        if (board_.grid_[row][col] == cell_t::Nobody) {
                            board_.grid_[row][col] = board_.x_turn_ ? cell_t::x : cell_t::o;
                            board_.x_turn_ = not board_.x_turn_;
                            auto win_pos = check_o_lose();
                            all_checked &= (win_pos.first != -1 && win_pos.second != -1);
                            board_.grid_[row][col] = cell_t::Nobody;
                            board_.x_turn_ = not board_.x_turn_;

                            if (board_.x_turn_ && (win_pos.first != -1 && win_pos.second != -1)) {
                                current = {row, col};
                                break;
                            }

                            if (not board_.x_turn_ && not all_checked) {
                                break;
                            }
                        }
                    }
                }

                if (all_checked && not board_.x_turn_) {
                    current = {0, 0};
                }
            }

            o_lose_.insert(std::make_pair(board_, current));
            return current;
        }

        return checked_board->second;
    }
}

void U10111::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
