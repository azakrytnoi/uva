#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10085.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>
#include <utility>
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10085 instance;
    instance();
}

namespace {

    struct element_t
    {
        char step_;
        int32_t state_;
        int32_t moves_;

        element_t(char c = 0, int32_t s = 0, int32_t m = 0) : step_(c), state_(s), moves_(m) {}
    };

    class state_t {
    public:
        state_t() : store_() {}

        void insert(int32_t state, const element_t& v)
        {
            store_[state % hash_][state] = v;
        }

        bool found(int32_t state)
        {
            auto ref(store_[state % hash_].find(state));
            auto res(ref != store_[state % hash_].end());
            return res;
        }

        void trace(std::ostream& out, int32_t state);

    private:
        constexpr static size_t hash_ = 131071;
        std::array <std::map<int32_t, element_t>, hash_> store_;
    };

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), puzzle_no_(0), state_(), work_(), board_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_, puzzle_no_;
        int32_t state_;
        std::unique_ptr<state_t> work_;
        std::array<std::array<int32_t, 3>, 3> board_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        sol.state_ = 0;

        for (auto i = 0u; i < 9u; ++i)
        {
            int32_t temp(0);
            in >> temp;
            sol.state_ = sol.state_ * 10 + temp;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Puzzle #" << sol.puzzle_no_ << std::endl;

        for (auto i = 0u; i < 3u; ++i)
        {
            for (auto j = 0u; j < 3u; ++j)
            {
                if (j > 0)
                {
                    out << ' ';
                }

                out << sol.board_[i][j];
            }

            out << std::endl;
        }

        sol.work_->trace(out, sol.state_);
        out << std::endl;
        return out;
    }

    std::pair<int32_t, int32_t> operator + (const std::pair<int32_t, int32_t>& lhs, const std::pair<int32_t, int32_t>& rhs)
    {
        return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
    }

    solution_t& solution_t::operator()()
    {
        static std::vector<std::pair<int32_t, int32_t>> delta({{-1, 0}, {1, 0}, {0, -1}, {0, 1}});
        static char dir_names[4] = {'U', 'D', 'L', 'R'};

        ++puzzle_no_;
        work_.reset(new state_t);
        work_->insert(state_, {});

        std::queue<element_t> q;
        q.push({0, state_, 0});
        auto count(0);

        typedef std::pair<int32_t, int32_t> position_t;
        position_t zero(0, 0);

        auto generate_board = [&](int32_t state)
        {
            auto temp(state);

            for (auto i = 2; i >= 0; --i)
            {
                for (auto j = 2; j >= 0; --j)
                {
                    board_[i][j] = temp % 10, temp /= 10;

                    if (board_[i][j] == 0)
                    {
                        zero.first = i, zero.second = j;
                    }
                }
            }
        };
        auto compact_board = [&]()
        {
            auto temp (0);

            for (auto j = 0; j < 3; ++j)
            {
                for (auto k = 0; k < 3; ++k)
                {
                    temp = temp * 10 + board_[j][k];
                }
            }

            return temp;
        };
        auto outside = [](const position_t& pos) -> bool
        {
            return pos.first < 0 || pos.second < 0 || pos.first >= 3 || pos.second >= 3;
        };

        while (not q.empty())
        {
            auto parent(q.front());
            q.pop();

            if (count <= parent.moves_)
            {
                count = parent.moves_, state_ = parent.state_;
            }

            generate_board(state_);

            for (auto i = 0; i < 4; i++)
            {
                auto step (zero + delta[i]);
                auto flip = [&]()
                {
                    std::swap(board_[zero.first][zero.second], board_[step.first][step.second]);
                };

                if (not outside(step))
                {
                    flip();
                    auto temp(compact_board());

                    if (not work_->found(temp))
                    {
                        work_->insert(temp, {dir_names[i], parent.state_});
                        q.push({0, temp, parent.moves_ + 1});
                    }

                    flip();
                }
            }
        }

        generate_board(state_);

        return *this;
    }

    void state_t::trace(std::ostream& out, int32_t state)
    {
        if (state != 0)
        {
            auto element = store_[state % hash_][state];
            trace(out, element.state_);

            if (element.state_ != 0)
            {
                out << element.step_;
            }
        }
    }

}

void U10085::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
