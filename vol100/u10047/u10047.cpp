#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10047.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <queue>
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
    U10047 instance;
    instance();
}

namespace {
    typedef std::pair<int16_t, int16_t> position_t;

    struct node_t
    {
        position_t pos_;
        uint8_t direction_, color_;
        int64_t step_;
        node_t(position_t& pos, uint8_t dir, uint8_t col, int64_t step): pos_(pos), direction_(dir), color_(col), step_(step) {}
    };

    position_t operator + (const position_t& lhs, const position_t& rhs)
    {
        position_t res(lhs);
        res.first += rhs.first;
        res.second += rhs.second;
        return res;
    }

    class solution_t {
    public:
        solution_t() : NCase_(0), M_(), N_(), field_(), start_(), target_(), time_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return N_ != 0 && M_ != 0;
        }
        solution_t& operator()();

    private:
        size_t NCase_;
        int16_t M_, N_;
        std::vector<std::string> field_;
        position_t start_, target_;
        int64_t time_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (in >> sol.N_ >> sol.M_ && sol.M_ > 0 && sol.N_ > 0)
        {
            sol.field_.clear();
            sol.field_.reserve(sol.N_);
            std::istream_iterator<std::string> sin(in);
            std::copy_n(sin, sol.N_, std::back_inserter(sol.field_));
            position_t pos(0, 0);
            std::for_each(sol.field_.begin(), sol.field_.end(), [&](auto & row)
            {
                pos.second = 0;
                std::for_each(row.begin(), row.end(), [&](auto ch)
                {
                    switch (ch)
                    {
                    case 'S':
                        sol.start_ = pos;
                        break;

                    case 'T':
                        sol.target_ = pos;
                        break;

                    default:
                        break;
                    }

                    ++pos.second;
                });
                ++pos.first;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Case #" << sol.NCase_ << std::endl;

        if (sol.time_ < 0)
        {
            out << "destination not reachable";
        }
        else
        {
            out << "minimum time = " << sol.time_ << " sec";
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        ++NCase_;
        time_ = -1;
        std::map<position_t, std::map<uint8_t, std::array<uint8_t, 5>>> dp;
        dp[start_][0][0] = 1;
        static const position_t d[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        std::queue<node_t> work;
        work.push({start_, 0, 0, 0});

        while (not work.empty())
        {
            auto node (work.front());
            work.pop();

            if (node.pos_ == target_ && node.color_ == 0)
            {
                time_ = node.step_;
                break;
            }

            position_t next(node.pos_ + d[node.direction_]);

            //try forward
            if (not (next.first < 0 || next.second < 0 || next.first >= N_ || next.second >= M_ || field_[next.first][next.second] == '#'))
            {
                uint8_t next_color((node.color_ + 1) % 5);

                if (dp[next][node.direction_][next_color] == 0)
                {
                    dp[next][node.direction_][next_color] = node.step_ + 1;
                    work.push({next, node.direction_, next_color, node.step_ + 1});
                }
            }

            // rotate right
            uint8_t next_dir((node.direction_ + 1) % 4);

            if (dp[node.pos_][next_dir][node.color_] == 0)
            {
                dp[node.pos_][next_dir][node.color_] = node.step_ + 1;
                work.push({node.pos_, next_dir, node.color_, node.step_ + 1});
            }

            // rotate left
            next_dir = (node.direction_ + 3) % 4;

            if (dp[node.pos_][next_dir][node.color_] == 0)
            {
                dp[node.pos_][next_dir][node.color_] = node.step_ + 1;
                work.push({node.pos_, next_dir, node.color_, node.step_ + 1});
            }
        }

        return *this;
    }

}

void U10047::operator()() const
{
    solution_t sol;
    bool next(false);

    while (std::cin >> sol && sol)
    {
        if (next)
        {
            std::cout << std::endl;
        }

        std::cout << sol() << std::endl;
        next = true;
    }
}
