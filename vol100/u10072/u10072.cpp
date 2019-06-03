#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10072.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>
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
    U10072 instance;
    instance();
}

namespace {

    struct player_t
    {
        enum class kind_t
        {
            battler, bowler, allrounder
        };

        std::map<kind_t, double_t> capacity_;
        std::map<kind_t, bool> capability_;

        player_t() : capacity_(), capability_() {}

        friend std::istream& operator >>(std::istream& in, player_t& rhs)
        {
            auto round = [](double_t d) -> int16_t
            {
                // auto ceil (std::ceil(d + 0.5));
                // auto floor(std::floor(d + 0.5));
                // return d - floor > ceil - d ? ceil : floor;
                return int16_t(d + 0.5);
            };
            int16_t bat(0), bow(0), fd(0);
            in >> bat >> bow >> fd;
            rhs.capacity_[player_t::kind_t::battler] = round(0.8 * bat + 0.2 * fd);
            rhs.capacity_[player_t::kind_t::bowler] = round(0.7 * bow + 0.1 * bat + 0.2 * fd);
            rhs.capacity_[player_t::kind_t::allrounder] = round(0.4 * bat + 0.4 * bow + 0.2 * fd);
            rhs.capability_[player_t::kind_t::battler] = bat && fd;
            rhs.capability_[player_t::kind_t::battler] = bow && (bat || fd);
            rhs.capability_[player_t::kind_t::allrounder] = bow && bat && fd;
            return in;
        }
    };

    struct team_t
    {
        typedef std::map<size_t, std::map<int16_t, std::map<int16_t, std::map<int16_t, size_t>>>> dp_matrix_t;

        uint16_t n_bat_, n_bow_, n_ar_;
        std::vector<player_t> players_;
        std::map<player_t::kind_t, std::vector<size_t>> roles_;

        team_t() : n_bat_(), n_bow_(), n_ar_(), players_(), roles_() {}

        operator bool () const
        {
            return not players_.empty();
        }

        double_t build();

        void prepare (dp_matrix_t& dp, dp_matrix_t& trace);
        void group_team(dp_matrix_t& trace);

        friend std::istream& operator >> (std::istream& in, team_t& rhs)
        {
            rhs.players_.clear();
            size_t n(0);

            if (in >> n && n > 0)
            {
                std::istream_iterator<player_t> pin(in);
                std::copy_n(pin, n, std::back_inserter(rhs.players_));
                in >> rhs.n_bat_ >> rhs.n_bow_ >> rhs.n_ar_;
            }

            return in;

        }

        friend std::ostream& operator << (std::ostream& out, const team_t& rhs)
        {
            out << "Batsmen :";
            auto& battlers = rhs.roles_.find(player_t::kind_t::battler)->second;
            std::for_each(battlers.begin(), battlers.end(), [&](auto idx)
            {
                out << ' ' << idx;
            });
            out << std::endl << "Bowlers :";
            auto& bowlers = rhs.roles_.find(player_t::kind_t::bowler)->second;
            std::for_each(bowlers.begin(), bowlers.end(), [&](auto idx)
            {
                out << ' ' << idx;
            });
            out << std::endl << "All-rounders :";
            auto& allrounders = rhs.roles_.find(player_t::kind_t::allrounder)->second;
            std::for_each(allrounders.begin(), allrounders.end(), [&](auto idx)
            {
                out << ' ' << idx;
            });
            return out;
        }
    };


    class solution_t {
    public:
        solution_t() : N_(), team_(), effective_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return team_;
        }
        solution_t& operator()();

    private:
        size_t N_;
        team_t team_;
        double_t effective_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.team_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Team #" << sol.N_ << std::endl << "Maximum Effective Score = " << sol.effective_ << std::endl;
        out << sol.team_ << std::endl;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        ++N_;
        effective_ = team_.build();
        return *this;
    }

    void team_t::prepare(dp_matrix_t& dp, dp_matrix_t& trace)
    {
        for (auto i = 0u; i < players_.size(); ++i)
        {
            for (auto ba = 0; ba <= n_bat_ + 1; ++ba)
            {
                for (auto bo = 0; bo <= n_bow_ + 1; ++bo)
                {
                    for (auto ar = 0; ar <= n_ar_ + 1; ++ar)
                    {
                        auto& curr (dp[i][ba][bo][ar]);
                        trace[i][ba][bo][ar] = 0;
                        curr = dp[i - 1][ba][bo][ar];

                        if (ba > 0 && players_[i].capability_[player_t::kind_t::battler])
                        {
                            auto next = dp[i - 1][ba - 1][bo][ar] + players_[i].capacity_[player_t::kind_t::battler];

                            if (next > curr)
                            {
                                curr = next;
                                trace[i][ba][bo][ar] = 1;
                            }
                        }

                        if (bo > 0 && players_[i].capability_[player_t::kind_t::bowler])
                        {
                            auto next = dp[i - 1][ba][bo - 1][ar] + players_[i].capacity_[player_t::kind_t::bowler];

                            if (next > curr)
                            {
                                curr = next;
                                trace[i][ba][bo][ar] = 2;
                            }
                        }

                        if (ar > 0 && players_[i].capability_[player_t::kind_t::allrounder])
                        {
                            auto next = dp[i - 1][ba][bo][ar - 1] + players_[i].capacity_[player_t::kind_t::allrounder];

                            if (next > curr)
                            {
                                curr = next;
                                trace[i][ba][bo][ar] = 3;
                            }
                        }

                    }
                }
            }
        }
    }

    void team_t::group_team(dp_matrix_t& trace)
    {
        auto ba(n_bat_), bo(n_bow_), ar(n_ar_);
        int32_t idx(players_.size());
        roles_.clear();

        while (idx >= 0)
        {
            auto k = trace[idx][ba][bo][ar];

            if (k == 1)
            {
                roles_[player_t::kind_t::battler].push_back(idx + 1);
                ba--;
            }
            else if (k == 2)
            {
                roles_[player_t::kind_t::bowler].push_back(idx + 1);
                bo--;
            }
            else if (k == 3)
            {
                roles_[player_t::kind_t::allrounder].push_back(idx + 1);
                ar--;
            }

            idx--;
        }

        std::sort(roles_[player_t::kind_t::battler].begin(), roles_[player_t::kind_t::battler].end());
        std::sort(roles_[player_t::kind_t::bowler].begin(), roles_[player_t::kind_t::bowler].end());
        std::sort(roles_[player_t::kind_t::allrounder].begin(), roles_[player_t::kind_t::allrounder].end());
    }



    double_t team_t::build()
    {
        dp_matrix_t dp, trace;
        prepare(dp, trace);
        group_team(trace);

        return  dp[players_.size() - 1][n_bat_][n_bow_][n_ar_];
    }

}

void U10072::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
