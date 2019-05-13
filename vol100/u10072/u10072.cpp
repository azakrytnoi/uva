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
        double_t bat_, bow_, fr_;
        bool ok_bat_, ok_bow_, ok_ar_;
        player_t() : bat_(), bow_(), fr_(), ok_bat_(), ok_bow_(), ok_ar_() {}

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
            rhs.bat_ = round(0.8 * bat + 0.2 * fd);
            rhs.bow_ = round(0.7 * bow + 0.1 * bat + 0.2 * fd);
            rhs.fr_ = round(0.4 * bat + 0.4 * bow + 0.2 * fd);
            rhs.ok_bat_ = bat && fd;
            rhs.ok_bow_ = bow && (bat || fd);
            rhs.ok_ar_ = bow && bat && fd;
            return in;
        }
    };

    struct team_t
    {
        uint16_t n_bat_, n_bow_, n_ar_;
        std::vector<player_t> players_;
        std::vector<size_t> bat_, bow_, ar_;

        team_t() : n_bat_(), n_bow_(), n_ar_(), players_(), bat_(), bow_(), ar_() {}

        operator bool () const
        {
            return not players_.empty();
        }

        double_t build();

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
            std::for_each(rhs.bat_.begin(), rhs.bat_.end(), [&](auto idx)
            {
                out << ' ' << idx;
            });
            out << std::endl << "Bowlers :";
            std::for_each(rhs.bow_.begin(), rhs.bow_.end(), [&](auto idx)
            {
                out << ' ' << idx;
            });
            out << std::endl << "All-rounders :";
            std::for_each(rhs.ar_.begin(), rhs.ar_.end(), [&](auto idx)
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

    double_t team_t::build()
    {
        std::map<size_t, std::map<int16_t, std::map<int16_t, std::map<int16_t, size_t>>>> dp, trace;

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

                        if (ba > 0 && players_[i].ok_bat_)
                        {
                            auto next = dp[i - 1][ba - 1][bo][ar] + players_[i].bat_;

                            if (next > curr)
                            {
                                curr = next;
                                trace[i][ba][bo][ar] = 1;
                            }
                        }

                        if (bo > 0 && players_[i].ok_bow_)
                        {
                            auto next = dp[i - 1][ba][bo - 1][ar] + players_[i].bow_;

                            if (next > curr)
                            {
                                curr = next;
                                trace[i][ba][bo][ar] = 2;
                            }
                        }

                        if (ar > 0 && players_[i].ok_ar_)
                        {
                            auto next = dp[i - 1][ba][bo][ar - 1] + players_[i].fr_;

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

        auto ba(n_bat_), bo(n_bow_), ar(n_ar_);
        int32_t idx(players_.size());
        bat_.clear();
        bow_.clear();
        ar_.clear();

        while (idx>=0)
        {
            auto k = trace[idx][ba][bo][ar];

            if (k == 1)
            {
                bat_.push_back(idx + 1);
                ba--;
            }
            else if (k == 2)
            {
                bow_.push_back(idx + 1);
                bo--;
            }
            else if (k == 3)
            {
                ar_.push_back(idx + 1);
                ar--;
            }

            idx--;
        }

        std::sort(bat_.begin(), bat_.end());
        std::sort(bow_.begin(), bow_.end());
        std::sort(ar_.begin(), ar_.end());

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
