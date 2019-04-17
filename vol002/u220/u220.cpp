#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u220.h"

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
    U220 instance;
    instance();
}

namespace {

    struct place_t
    {
        place_t(uint8_t x = 0, uint8_t y = 0) : x_(x), y_(y) {}

        friend std::ostream& operator<<(std::ostream& out, const place_t& place)
        {
            out << '(' << static_cast<char>(place.x_ + '0') << ',' << static_cast<char>(place.y_ + '0') << ')';
            return out;
        }

        uint8_t x_;
        uint8_t y_;
    };

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), board_(), commands_(), side_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::vector<std::string> board_;
        std::vector<std::pair<std::string, std::string>> commands_;
        uint8_t side_;

        void possible_moves(std::vector<place_t>& moves);
        void apply_move(const uint8_t side, const uint8_t x, const uint8_t y);
        std::string stats();

        void apply_in_direction(int16_t x, int16_t y, const int16_t dx, const int16_t dy, char desired);
        bool check_direction(int16_t x, int16_t y, const int16_t dx, const int16_t dy, const uint8_t side);

        bool withIn (int16_t x, int16_t y)
        {
            return x >= 0 && x < BOARD_SIZE_ && y >= 0 && y < BOARD_SIZE_;
        };

        bool check (int16_t x, int16_t y, const int16_t dx, const int16_t dy, const uint8_t side)
        {
            if (not withIn(x, y))
            {
                return false;
            }

            if (board_[x][y] == players_[side])
            {
                return true;
            }
            else if (board_[x][y] == '-')
            {
                return false;
            }

            return check(x + dx, y + dy, dx, dy, side);
        };

        const char players_[2] = { 'W', 'B' };
        static const int BOARD_SIZE_ = 8;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        sol.board_.clear();
        sol.commands_.clear();

        if (in && sol.N_ > 0)
        {
            sol.board_.reserve(sol.BOARD_SIZE_);
            std::string line(" ");
            std::generate_n(std::back_inserter(sol.board_), sol.BOARD_SIZE_, [&]()
            {
                in >> line;
                return line;
            });
            in >> line;
            sol.side_ = line[0] == 'W' ? 0 : 1;

            while (in >> line && line != "Q")
            {
                sol.commands_.push_back(std::make_pair(line, ""));
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<std::string> iout(out, "\n");
        std::transform(sol.commands_.begin(), sol.commands_.end(), iout, [](const std::pair<std::string, std::string>& command)
        {
            return command.second;
        });
        std::copy(sol.board_.begin(), sol.board_.end(), iout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<place_t> possibleMoves;
        possible_moves(possibleMoves);

        for (auto& command : commands_)
        {
            switch (command.first[0])
            {
            case 'L':
            {
                if (possibleMoves.empty())
                {
                    command.second = "No legal move.";
                }
                else
                {
                    std::stringstream temp;
                    std::ostream_iterator<std::string> itemp(temp, " ");
                    std::transform(possibleMoves.begin(), possibleMoves.end(), itemp, [](const place_t& place)
                    {
                        std::stringstream ss;
                        ss << place;
                        return ss.str();
                    });
                    command.second = temp.str();
                    command.second.erase(command.second.find_last_of(' '));
                }
            }
            break;

            case 'M':
            {
                if (possibleMoves.empty())
                {
                    apply_move(1 - side_, command.first[1] - '1', command.first[2] - '1');
                }
                else
                {
                    apply_move(side_, command.first[1] - '1', command.first[2] - '1');
                    side_ = 1 - side_;
                }

                possible_moves(possibleMoves);
                command.second = stats();
            }
            break;

            default:
                break;
            }
        }

        return *this;
    }

    void solution_t::apply_move(const uint8_t side, const uint8_t x, const uint8_t y)
    {
        for (int16_t dy = -1; dy < 2; dy++)
        {
            for (int16_t dx = -1; dx < 2; dx++)
            {
                if (check_direction (x, y, dx, dy, side))
                {
                    apply_in_direction (x + dx, y + dy, dx, dy, players_[side]);
                }
            }
        }

        board_[x][y] = players_[side];
    }

    void solution_t::apply_in_direction(int16_t x, int16_t y, const int16_t dx, const int16_t dy, const char desired)
    {
        while (board_[x][y] != desired)
        {
            board_[x][y] = desired;
            x += dx;
            y += dy;
        }
    }

    void solution_t::possible_moves(std::vector<place_t>& moves)
    {
        moves.clear();

        for (int16_t y = 0; y < BOARD_SIZE_; y++)
        {
            for (int16_t x = 0; x < BOARD_SIZE_; x++)
            {
                bool valid(false);

                for (int16_t dy = -1; dy < 2 && not valid; dy++)
                {
                    for (int16_t dx = -1; dx < 2; dx++)
                    {
                        if (check_direction (x, y, dx, dy, side_))
                        {
                            valid = true;
                            break;
                        }
                    }
                }

                if (valid)
                {
                    moves.push_back(place_t(x + 1, y + 1));
                }
            }
        }

        std::sort(moves.begin(), moves.end(), [](const place_t& lhs, const place_t& rhs)
        {
            if (lhs.x_ == rhs.x_)
            {
                return lhs.y_ < rhs.y_;
            }

            return lhs.x_ < rhs.x_;
        });
    }

    bool solution_t::check_direction(int16_t x, int16_t y, const int16_t dx, const int16_t dy, const uint8_t side)
    {
        if (dx == 0 && dy == 0)
        {
            return false;
        }

        if (board_[x][y] != '-')
        {
            return false;
        }

        x += dx;
        y += dy;

        if (not withIn(x, y) || board_[x][y] == players_[side])
        {
            return false;
        }

        return check(x, y, dx, dy, side);
    }

    std::string solution_t::stats()
    {
        std::pair<uint16_t, uint16_t> counter = std::accumulate(board_.begin(), board_.end(), std::make_pair(0, 0), //
                                                [](const std::pair<uint16_t, uint16_t>& prev, const std::string & row)
        {
            std::pair<uint16_t, uint16_t> inner = std::accumulate(row.begin(), row.end(), prev, [](std::pair<uint16_t, uint16_t>& running, const char cell)
            {
                if (cell == 'W')
                {
                    running.first++;
                }
                else if (cell == 'B')
                {
                    running.second++;
                }

                return running;
            });
            return inner;
        });
        std::stringstream str;
        str << "Black - " << std::right << std::setw(2) << counter.second << " White - " << std::right << std::setw(2) << counter.first;
        return str.str();
    }

}

void U220::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
