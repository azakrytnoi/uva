#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#endif

#include "u10194.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <regex>
#include <cstdlib>
#include <memory>

namespace
{

class team
{
public:
    explicit team(std::string& name) : points_(0), wins_(0), ties_(0), losses_(0), goals_scored_(0), goals_against_(0), name_(name) {}

    friend std::ostream& operator << (std::ostream& out, team& t)
    {
        out << t.name() << " " << t.points() << "p, "
            << t.games_played() << "g (" << t.wins() << "-" << t.ties() << "-" << t.losses() << "), "
            << t.goals() << "gd (" << t.goals_scored() << "-" << t.goals_against() << ")";
        return out;
    }

    bool operator < (const team& other)
    {
        if (points_ == other.points_) {
            if (wins_ == other.wins_) {
                if (goals() == other.goals()) {
                    if (goals_scored_ == other.goals_scored_) {
                        if (games_played() == other.games_played()) {
                            return name_ > other.name_;
                        } else {
                            return games_played() > other.games_played();
                        }
                    } else {
                        return goals_scored_ < other.goals_scored_;
                    }
                } else {
                    return goals() < other.goals();
                }
            } else {
                return wins_ < other.wins_;
            }
        } else {
            return points_ < other.points_;
        }
    }

    int& points()
    {
        return points_;
    }
    int games_played() const
    {
        return wins_ + losses_ + ties_;
    }
    int& wins()
    {
        return wins_;
    }
    int& ties()
    {
        return ties_;
    }
    int& losses()
    {
        return losses_;
    }
    int goals() const
    {
        return goals_scored_ - goals_against_;
    }
    int& goals_scored()
    {
        return goals_scored_;
    }
    int& goals_against()
    {
        return goals_against_;
    }
    const std::string& name() const
    {
        return name_;
    }

    void win()
    {
        wins_++;
        points_ += 3;
    }
    void loss()
    {
        losses_++;
    }
    void tie()
    {
        ties_++;
        points_ += 1;
    }

    static void recordResults(team& team1, team& team2, int team1Goals, int team2Goals);
private:
    int points_;
    int wins_;
    int ties_;
    int losses_;
    int goals_scored_;
    int goals_against_;
    const std::string name_;
};

void team::recordResults(team & team1, team & team2, int team1Goals, int team2Goals)
{
    team1.goals_scored() += team1Goals;
    team2.goals_scored() += team2Goals;
    team1.goals_against() += team2Goals;
    team2.goals_against() += team1Goals;
    if (team1Goals < team2Goals) {
        team2.win();
        team1.loss();
    } else if (team1Goals > team2Goals) {
        team1.win();
        team2.loss();
    } else {
        team1.tie();
        team2.tie();
    }
}
}

U10194::U10194()
{
}

U10194::~U10194()
{
}

void U10194::operator()()
{
    int N;
    std::string line;
    std::map<std::string, std::shared_ptr<team>> teams;
    std::vector<std::shared_ptr<team>> results;
    std::regex reg("([^#]+)#([^@]+)@([^#]+)#(.*)");
    std::smatch match;
    std::cin >> N;
    std::getline(std::cin, line);
    while (N--) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        int n;
        std::cin >> n;
        results.clear();
        results.reserve(n);
        teams.clear();
        std::getline(std::cin, line);
        std::generate_n(std::back_inserter(results), n, [&]() {
            std::getline(std::cin, line);
            auto t = std::make_shared<team>(line);
            teams[line] = t;
            return t;
        });
        std::cin >> n;
        std::getline(std::cin, line);
        while (n--) {
            std::getline(std::cin, line);
            if (std::regex_match(line, match, reg)) {
                if (match.size() == 5) {
                    int team1Goals = std::atoi(match[2].str().c_str());
                    int team2Goals = std::atoi(match[3].str().c_str());
                    team::recordResults(*teams[match[1].str()], *teams[match[4].str()], team1Goals, team2Goals);
                }
            }
        }
        std::sort(results.begin(), results.end(), [](auto t1, auto t2) {
            return *t2 < *t1;
        });
        int position = 0;
        std::for_each(results.begin(), results.end(), [&position](auto t) {
            std::cout << (++position) << ") " << *t << std::endl;
        });
        std::cout << std::endl;
    }
}
