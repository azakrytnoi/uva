#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10258.h"

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <iterator>

namespace std {
    ostream& operator << (ostream& out, const tuple<int, int, int>& t)
    {
        out << get<0>(t) << " " << get<1>(t) << " " << get<2>(t);
        return out;
    }
}

U10258::U10258()
{}

U10258::~U10258()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10258 instance;
    instance();
}
void U10258::operator()()
{
    int N;
    std::cin >> N;
    std::string line;
    std::map<int, std::map<int, std::pair<bool, int>>> contest;
    std::vector<std::tuple<int, int, int>> scoreboard;
    std::getline(std::cin, line);
    std::getline(std::cin, line);

    while (N--) {
        contest.clear();
        scoreboard.clear();

        while (std::getline(std::cin, line) && !line.empty()) {
            int cont, problem, time;
            char resolution;
            {
                std::stringstream ss(line);
                ss >> cont >> problem >> time >> resolution;
            }

            if (!contest[cont][problem].first) {
                switch (resolution) {
                case 'C':
                    contest[cont][problem].first = true;
                    contest[cont][problem].second += time;
                    break;

                case 'I':
                    contest[cont][problem].second += 20;
                    break;

                default:
                    break;
                }
            }
        }

        scoreboard.reserve(contest.size());
        std::for_each(contest.begin(), contest.end(), [&scoreboard](auto participant) {
            int total(0), problems(0);
            std::for_each(participant.second.begin(), participant.second.end(), [&total, &problems](auto problem) {
                if (problem.second.first) {
                    total += problem.second.second;
                    problems++;
                }
            });
            scoreboard.push_back(std::make_tuple(participant.first, problems, total));
        });
        std::sort(scoreboard.begin(), scoreboard.end(), [](auto t1, auto t2) {
            if (std::get<1>(t1) == std::get<1>(t2)) {
                if (std::get<2>(t1) == std::get<2>(t2)) {
                    return std::get<0>(t1) < std::get<0>(t2);

                } else {
                    return std::get<2>(t1) < std::get<2>(t2);
                }

            } else {
                return std::get<1>(t1) > std::get<1>(t2);
            }
        });
        {
            std::ostream_iterator<std::tuple<int, int, int>> out(std::cout, "\n");
            std::copy(scoreboard.begin(), scoreboard.end(), out);
            std::cout << std::endl;
        }
    }
}