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

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U207 instance;
    instance();
}

namespace {

    typedef long double REAL;

    class purse {
    public:
        REAL value_;
        std::array<REAL, 70> distibution_;
        purse(): value_(), distibution_() {}

        friend std::istream& operator >>(std::istream& in, purse& p)
        {
            in >> p.value_;
            std::generate_n(p.distibution_.begin(), 70, [&]() {
                REAL tmp;
                in >> tmp;
                return tmp;
            });
            return in;
        }
    };

    class player {
    public:
        std::string name_;
        bool amateur_;
        std::array<uint32_t, 4> scores_;
        bool disqualified_;
        uint32_t total_;
        uint16_t place_;
        REAL prize_;
        player() : name_(), amateur_(false), scores_(), disqualified_(), total_(), place_(), prize_() {}

        friend std::istream& operator>>(std::istream& in, player& p)
        {
            p.place_ = 0;
            std::string line;
            std::getline(in, line);
            p.name_ = line.substr(0, 20);
            p.name_.erase(p.name_.find_last_not_of(' ') + 1);
            p.amateur_ = p.name_[p.name_.length() - 1] == '*';
            std::stringstream score_stream(line.substr(21));
            std::istream_iterator<std::string> iscore_stream (score_stream);
            std::vector<std::string> scores (iscore_stream, std::istream_iterator<std::string>());
            std::transform(scores.begin(), scores.end(), p.scores_.begin(), [&](auto & score) {
                char* tmp;
                return std::strtol(score.c_str(), &tmp, 0);
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

        void play_round(size_t round_no, std::vector<std::shared_ptr<player>>::iterator end);

        purse purse_;
        std::vector<std::shared_ptr<player>> players_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::string line;

        if (std::getline(in, line)) {
            in >> sol.purse_;
            uint32_t n_players;
            in >> n_players;
            std::getline(in, line);
            sol.players_.clear();
            sol.players_.reserve(n_players);
            std::generate_n(std::back_inserter(sol.players_), n_players, [&]() {
                auto p = std::make_shared<player>();
                in >> *p;
                return p;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << R"(
Player Name          Place     RDJ RD2 RD3 RD4 TOTAL         Money Won
-----------------------------------------------------------------------
)";
        std::for_each(sol.players_.begin(), sol.players_.end(), [&](auto pl)
        {
            out << std::setw(20) << std::left << pl->name_ << " ";

            if (pl->place_ != 0) {
            	out << std::setw(10) << pl->place_ ;
            } else {
            	out << std::setw(10) << ' ';
            }

            std::for_each(pl->scores_.begin(), pl->scores_.end(), [&](auto score)
            {
            	if (score != 0){
            		out << std::setw(4) << score;
            	} else {
            		out << std::setw(4) << ' ';
            	}
            });

            if (not pl->disqualified_) {
                out << std::setw(12) << pl->total_;
            } else {
                out << std::setw(12) << "DQ";
            }

            if (pl->prize_ > 0) {
                out << "$" << std::setw(11) << std::right << std::fixed << std::setprecision(2) << pl->prize_;
            }

            out << std::endl;
        });

        return out;
    }

    void solution::play_round(size_t round_no, std::vector<std::shared_ptr<player>>::iterator end)
    {
    	std::for_each(players_.begin(), end, [&](auto pl) {
    		pl->total_ += pl->scores_[round_no];
    		pl->disqualified_ = pl->scores_[round_no] == 0;
    		if (pl->disqualified_) pl->place_ = 0;
    	});

    	std::sort(players_.begin(), end, [](auto p1, auto p2)
    	{
    		if (p1->disqualified_ || p2->disqualified_) {

    			if (p1->disqualified_ && p2->disqualified_) {
    				return std::count_if(p1->scores_.begin(), p1->scores_.end(), [](auto score) { return score != 0;}) > std::count_if(p2->scores_.begin(), p2->scores_.end(), [](auto score) { return score != 0;});
    			} else {
    				return p2->disqualified_;
    			}

    		}

    		return p1->total_ < p2->total_;
    	});
    }

    solution& solution::operator()()
    {
    	play_round(0, players_.end());
    	play_round(1, std::find_if(players_.begin(), players_.end(), [](auto pl) { return pl->disqualified_; }));

    	uint32_t last_score(0);
    	uint16_t place (0);
    	for(auto pl : players_) {
    		if (not pl->disqualified_ && place <= purse_.distibution_.size()) {
    			if (pl->total_ != last_score) {
    				last_score = pl->total_;
    				place++;
    			}
    			pl->place_ = place;
    		}
    	}

    	play_round(2, std::find_if(players_.begin(), players_.end(), [](auto pl) { return pl->disqualified_; }));
    	play_round(3, std::find_if(players_.begin(), players_.end(), [](auto pl) { return pl->disqualified_; }));

    	auto perc = purse_.distibution_.begin();
    	place = 0;
    	last_score = 0;
    	for(auto pl : players_)
    	{
    		if(not pl->disqualified_ && pl->place_ != 0) {
    			if (last_score != pl->total_) {
    				place++;
    				last_score = pl->total_;
    			}
    			pl->place_ = place;
    		} else {
    			pl->place_ = 0;
    		}
    	}

    	std::vector<std::shared_ptr<player>> ties;
    	ties.reserve(players_.size());
    	REAL tie_prc(0);
    	for(auto pl = players_.begin(); pl != players_.end(); ++pl)
    	{
    		if ((*pl)->place_ == 0) continue;
    		if (pl != players_.begin() && (*pl)->place_ == (*(pl - 1))->place_) { // @suppress("Field cannot be resolved")

    			if (ties.empty()) {
    				--perc;
    				tie_prc = *perc;
    				(*(pl - 1))->prize_ = 0;
    				ties.push_back(*(pl - 1));
    			}

    			ties.push_back(*pl);
				if (perc != purse_.distibution_.end()) {
					(*pl)->prize_ = 0;
					tie_prc += *perc;
					++perc;
				}
    		} else {
    			if (not ties.empty()) {
    				tie_prc /= ties.size();
    				for (auto tie = ties.begin(); tie != ties.end(); ++tie) {
    					if (not (*tie)->amateur_) { // @suppress("Field cannot be resolved")
    						(*tie)->prize_ = purse_.value_ * tie_prc;
    					}
    				}
    				ties.clear();
    			}

        		if (not (*pl)->amateur_ && perc != purse_.distibution_.end()) { // @suppress("Field cannot be resolved")
        			(*pl)->prize_ = purse_.value_ * *perc;
        			++perc;
        		}
    		}
    	}

        return *this;
    }

}

void U207::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
