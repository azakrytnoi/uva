#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u207.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <array>
#include <sstream>
#include <memory>
#include <cstdint>
#include <charconv>
#include <decimal/decimal>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U207 instance;
    instance();
}

namespace {

    typedef std::decimal::decimal32 REAL;

    class purse_t {
    public:
        REAL value_;
        std::array<REAL, 70> distibution_;
        purse_t(): value_(), distibution_() {}

        friend std::istream& operator >>(std::istream& in, purse_t& p)
        {
            double tmp;
            in >> tmp;
            p.value_ = REAL(tmp);
            std::generate_n(p.distibution_.begin(), 70, [&]()
            {
                double tmp_val;
                in >> tmp_val;
                return REAL(tmp_val); // / 100.0;
            });
            return in;
        }
    };

    class player_t {
    public:
        std::string name_;
        bool tie_;
        bool amateur_;
        std::array<uint32_t, 4> scores_;
        bool disqualified_;
        uint32_t total_;
        uint16_t place_;
        REAL prize_;
        player_t() : name_(), tie_(false), amateur_(false), scores_(), disqualified_(), total_(), place_(), prize_() {}

        friend std::istream& operator>>(std::istream& in, player_t& p)
        {
            p.place_ = 0;
            std::string line;
            std::getline(in, line);
            p.name_ = line.substr(0, 20);
            p.name_.erase(p.name_.find_last_not_of(' ') + 1);
            p.amateur_ = p.name_[p.name_.length() - 1] == '*';
            std::stringstream score_stream(line.substr(21));
            std::istream_iterator<std::string> iscore_stream (score_stream);
            // std::vector<std::string> scores (iscore_stream, std::istream_iterator<std::string>());
            std::transform(iscore_stream, std::istream_iterator<std::string>(), p.scores_.begin(), [&](auto & score)
            {
                uint16_t result;
                std::from_chars(score.data(), score.data() + score.size(), result);
                return result;
            });

            p.disqualified_ = false;
            p.total_ = 0;

            return in;
        }
    };

    class solution {
    public:
        solution() : purse_(), players_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        void play_round(size_t round_no, std::vector<std::shared_ptr<player_t>>::iterator end);

        purse_t purse_;
        std::vector<std::shared_ptr<player_t>> players_;

        void cut_36();
        void simulate();
        void set_places();
        void calc_prizes();
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::string line;

        if (std::getline(in, line))
        {
            in >> sol.purse_;
            uint32_t n_players;
            in >> n_players;
            std::getline(in, line);
            sol.players_.clear();
            sol.players_.reserve(n_players);
            std::generate_n(std::back_inserter(sol.players_), n_players, [&]()
            {
                auto p = std::make_shared<player_t>();
                in >> *p;
                return p;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << R"(Player Name          Place     RD1  RD2  RD3  RD4  TOTAL     Money Won
-----------------------------------------------------------------------
)";
        for (auto& pl : sol.players_)
        {
            out << std::setw(20) << std::left << pl->name_ << " ";

            if (pl->place_ != 0)
            {
                std::stringstream tmp;
                tmp << pl->place_;

                if (pl->tie_)
                {
                    tmp << 'T';
                }

                out << std::setw(10) << tmp.str() ;
            }
            else
            {
                out << std::setw(10) << ' ';
            }

            for (auto score : pl->scores_)
            {
                if (score != 0)
                {
                    out << std::setw(5) << score;
                }
                else
                {
                    out << std::setw(5) << ' ';
                }
            }

            if (not pl->disqualified_)
            {
                out << std::setw(3) << pl->total_;
            }
            else
            {
                out << std::setw(2) << "DQ";
            }

            if (pl->prize_ > 0)
            {
                out << "       $" << std::setw(9) << std::right << std::fixed << std::setprecision(2)
                    << std::decimal::decimal_to_double(pl->prize_);
            }

            out << std::endl;
        }

        return out;
    }

    void solution::play_round(size_t round_no, std::vector<std::shared_ptr<player_t>>::iterator end)
    {
        for (auto& pl : players_)
        {
            pl->total_ += pl->scores_[round_no];
            pl->disqualified_ = pl->scores_[round_no] == 0;

            if (pl->disqualified_)
            {
                pl->place_ = 0;
            }
        }

        std::sort(players_.begin(), end, [](auto p1, auto p2)
        {
            if (p1->disqualified_ || p2->disqualified_)
            {
                if (p1->disqualified_ && p2->disqualified_)
                {
                    return std::count_if(p1->scores_.begin(), p1->scores_.end(), [](auto score)
                    {
                        return score != 0;
                    }) > std::count_if(p2->scores_.begin(), p2->scores_.end(), [](auto score)
                    {
                        return score != 0;
                    });
                }
                else
                {
                    return p2->disqualified_;
                }
            }

            if (p1->total_ == p2->total_)
            {
                return p1->name_ < p2->name_;
            }

            return p1->total_ < p2->total_;
        });
    }

    void solution::cut_36()
    {
        std::vector<std::shared_ptr<player_t>> cut;
        cut.reserve(players_.size());
        std::copy_if(players_.begin(), players_.end(), std::back_inserter(cut), [&](auto pl)
        {
            return not pl->disqualified_ && (cut.size() < purse_.distibution_.size() || pl->total_ == cut.back()->total_);
        });
        players_.assign(cut.begin(), cut.end());
    }

    void solution::simulate()
    {
        auto last_player = [&]
        {
            return std::find_if(players_.begin(), players_.end(), [](auto pl)
            {
                return pl->disqualified_;
            });
        };

        play_round(0, players_.end());
        play_round(1, last_player());

        cut_36();

        play_round(2, last_player());
        play_round(3, last_player());
    }

    void solution::set_places()
    {
        uint16_t place (0);

        for (auto& pl : players_)
        {
            if (not pl->disqualified_)
            {
                pl->place_ = ++place;
            }
            else
            {
                pl->place_ = 0;
            }
        }
    }

    void solution::calc_prizes()
    {
        std::vector<std::shared_ptr<player_t>> ties;
        ties.reserve(players_.size());
        REAL tie_prc(0);
        auto perc = purse_.distibution_.begin();
        auto pl = players_.begin();

        auto collect_ties = [&]()
        {
            if (ties.empty())
            {
                tie_prc = (*(pl - 1))->amateur_ ? REAL(0) : *(perc - 1);
                ties.push_back(*(pl - 1));
            }

            ties.push_back(*pl);
            (*pl)->place_ = ties.front()->place_;

            if (not (*pl)->amateur_ && perc != purse_.distibution_.end())
            {
                tie_prc += *perc;
                ++perc;
            }
        };

        auto distribute_ties = [&]()
        {
            std::vector<std::shared_ptr<player_t>> clean;
            clean.reserve(ties.size());
            std::copy_if(ties.begin(), ties.end(), std::back_inserter(clean), [](auto pl)
            {
                return not pl->amateur_;
            });

            if (not clean.empty())
            {
                tie_prc /= ties.size();
                auto tie_amount (purse_.value_ * tie_prc);
                tie_amount *= ties.size();
                tie_amount /= REAL(clean.size() * 100.0);

                for (auto& tie : clean)
                {
                    tie->prize_ = tie_amount;
                    tie->tie_ = clean.size() > 1;
                }
            }

            ties.clear();
        };

        for (; pl != players_.end(); ++pl)
        {
            if ((*pl)->place_ == 0)
            {
                continue;
            }

            if (pl != players_.begin() && (*pl)->total_ == (*(pl - 1))->total_)   // @suppress("Field cannot be resolved")
            {
                collect_ties();
            }
            else
            {
                if (not ties.empty())
                {
                    distribute_ties();
                }

                if (not (*pl)->amateur_ && perc != purse_.distibution_.end())   // @suppress("Field cannot be resolved")
                {
                    (*pl)->prize_ = (purse_.value_ * *perc) / REAL(100.0);
                    ++perc;
                }
            }
        }
    }

    solution& solution::operator()()
    {
        simulate();
        set_places();
        calc_prizes();

        return *this;
    }

}

void U207::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
