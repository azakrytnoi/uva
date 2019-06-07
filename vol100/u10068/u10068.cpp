#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10068.h"

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
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10068 instance;
    instance();
}

namespace {
    typedef std::pair<int16_t, int16_t> position_t;

    position_t operator + (const position_t& lhs, const position_t& rhs)
    {
        position_t res(lhs);
        res.first += rhs.first;
        res.second += rhs.second;
        return res;
    }

    enum class direction_t : char
    {
        N = 'N', E = 'E', W = 'W', S = 'S'
    };

    struct threasure_t
    {
        size_t seq_;
        int32_t pick_, carry_;

        threasure_t(int32_t pick = 0, int32_t carry = 0) : seq_(), pick_(pick), carry_(carry) { }

        bool operator == (const threasure_t& rhs) const
        {
            return seq_ == rhs.seq_;
        }

        friend std::istream& operator >>(std::istream& in, threasure_t& rhs)
        {
            in >> rhs.pick_ >> rhs.carry_;
            return in;
        }
    };

    struct node_t
    {
        size_t dist_;
        size_t step_;
        node_t* prev_;
        node_t(size_t dist = std::numeric_limits<uint16_t>::max(), size_t step = 0, node_t* prev = nullptr)
            : dist_(dist), step_(step), prev_(prev) {}
    };

    class solution_t {
    public:
        solution_t() : n_case_(0), R_(), C_(), maze_(), start_(), target_(), threasures_(), step_(), energy_(), trace_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return (R_ + C_) != 0;
        }
        solution_t& operator()();

    private:
        size_t n_case_;
        int16_t R_, C_;
        std::vector<std::string> maze_;
        position_t start_, target_;
        std::map<position_t, threasure_t> threasures_;
        size_t step_;
        int64_t energy_;
        std::string trace_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.maze_.clear();
        sol.threasures_.clear();

        if (in >> sol.R_ >> sol.C_ && (sol.R_ + sol.C_) != 0)
        {
            sol.maze_.reserve(sol.R_);
            std::istream_iterator<std::string> sin(in);
            std::copy_n(sin, sol.R_, std::back_inserter(sol.maze_));
            in >> sol.step_;

            for (auto row = sol.maze_.begin(); row != sol.maze_.end(); ++row)
            {
                for (auto col = row->begin(); col != row->end(); ++col)
                {
                    switch (*col)
                    {
                    case 'S':
                        sol.start_ = {std::distance(sol.maze_.begin(), row), std::distance(row->begin(), col)};
                        break;

                    case 'T':
                        sol.target_ = {std::distance(sol.maze_.begin(), row), std::distance(row->begin(), col)};
                        break;

                    case '*':
                        in >> sol.threasures_[ {std::distance(sol.maze_.begin(), row), std::distance(row->begin(), col)}];
                        break;

                    default:
                        break;
                    }
                }
            }

            auto seq(0u);

            for (auto& thr : sol.threasures_)
            {
                thr.second.seq_ = seq++;
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Hunt #" << sol.n_case_ << std::endl;

        if (sol.energy_  < 0)
        {
            out << "The hunt is impossible" << std::endl;
        }
        else
        {
            out << "Minimum energy requirede = " << sol.energy_ << " cal" << std::endl;
            out << sol.trace_ << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        ++n_case_;
        energy_ = -1;
        std::queue<std::pair<position_t, size_t>> work;
        work.push({start_, 0u});
        static position_t deltas[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        std::map < position_t, std::pair < std::array < node_t, 1 << 11 >, std::array<bool, 1 << 11>>> dp;
        dp[start_].first[0].dist_ = 0;
        std::vector<threasure_t*> th(threasures_.size());
        std::transform(threasures_.begin(), threasures_.end(), th.begin(), [](auto & lhs)
        {
            return &lhs.second;
        });
        std::vector<size_t> weigths(1 << th.size());

        for (auto i = (1 << th.size()); i >= 0; --i)
        {
            auto sum(0u);

            for (auto j = 0u; j < th.size(); ++j)
            {
                if (((i >> j) & 1) != 0)
                {
                    sum += th[j]->carry_;
                }
            }

            weigths[i] = sum;
        }

        while (not work.empty())
        {
            auto node (work.front());
            work.pop();
            dp[node.first].second[node.second] = false;
            auto idx(0u);

            for (auto& delta : deltas)
            {
                auto next(node.first + delta);

                if (next.first < 0 || next.second < 0 || next.first >= R_ || next.second >= C_)
                {
                    continue;
                }

                if (maze_[next.first][next.second] == '#')
                {
                    continue;
                }

                auto thr(node.second);

                if (dp[next].first[thr].dist_ > dp[node.first].first[node.second].dist_ + weigths[node.second] + step_)
                {
                    dp[next].first[thr].dist_ = dp[node.first].first[node.second].dist_ + weigths[node.second] + step_;
                    dp[next].first[thr].prev_ = &dp[node.first].first[node.second];
                    dp[next].first[thr].step_ = 1 << idx;

                    if (not dp[next].second[thr])
                    {
                        dp[next].second[thr] = true;
                        work.push({next, thr});
                    }
                }

                auto possible(threasures_.find(next));

                if (possible != threasures_.end())
                {
                    thr = node.second | (1 << possible->second.seq_);

                    if (dp[next].first[thr].dist_ > dp[node.first].first[node.second].dist_ + weigths[node.second] + step_ + possible->second.pick_)
                    {
                        dp[next].first[thr].dist_ = dp[node.first].first[node.second].dist_ + weigths[node.second] + step_ + possible->second.pick_;
                        dp[next].first[thr].prev_ = &dp[node.first].first[node.second];
                        dp[next].first[thr].step_ = (1 << idx) | (1 << 4);

                        if (not dp[next].second[thr])
                        {
                            dp[next].second[thr] = true;
                            work.push({next, thr});
                        }
                    }
                }
                ++idx;
            }
        }

        if (dp[target_].first[1 << (threasures_.size() - 1)].dist_ != std::numeric_limits<uint16_t>::max())
        {
            static direction_t directions[] = {direction_t::N, direction_t::W, direction_t::E, direction_t::S};
            energy_ = dp[target_].first[1 << (threasures_.size() - 1)].dist_;
            node_t* ptr(&dp[target_].first[1 << (threasures_.size() - 1)]);
            trace_.clear();

            while (ptr != nullptr)
            {
                if (((ptr->step_ >> 4) & 1) != 0)
                {
                    trace_ += 'P' ;
                }

                for (auto i = 0u; i < 4u; ++i)
                {
                    if (((ptr->step_ >> i) & 1) != 0)
                    {
                        trace_ +=  static_cast<char>(directions[i]);
                    }
                }

                ptr = ptr->prev_;
            }

            std::reverse(trace_.begin(), trace_.end());
        }

        return *this;
    }

}

void U10068::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
