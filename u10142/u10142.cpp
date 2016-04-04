#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10142.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <deque>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10142 instance;
    instance();
}

namespace
{

class solution
{
public:
    solution();

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    solution& operator ()();

private:
    std::vector<std::string> candidates_;
    std::vector<std::string> winners_;
    std::vector<std::deque<size_t>> votes_;
};

solution::solution() :
    candidates_(), winners_(), votes_()
{
}

std::istream& operator >>(std::istream& in, solution& sol)
{
    sol.winners_.clear();
    sol.candidates_.clear();
    sol.votes_.clear();
    int n;
    in >> n;
    sol.candidates_.reserve(n);
    sol.winners_.reserve(n);
    std::string line;
    std::getline(in, line);
    std::generate_n(std::back_inserter(sol.candidates_), n, [&]() -> std::string {
        std::getline(in, line);
        return line;
    });
    while (std::getline(in, line) && !line.empty()) {
        std::stringstream iss(line);
        std::istream_iterator<size_t> iit(iss);
        sol.votes_.push_back(std::deque<size_t>());
        std::for_each(iit, std::istream_iterator<size_t>(), [&](size_t idx) {
            sol.votes_.back().push_back(idx);
        });
    }
    sol.votes_.shrink_to_fit();
    return in;
}

std::ostream& operator <<(std::ostream& out, const solution& sol)
{
    std::ostream_iterator<std::string> oit(out, "\n");
    std::copy(sol.winners_.begin(), sol.winners_.end(), oit);
    return out;
}

solution& solution::operator ()()
{
    if (candidates_.size() > 1) {
        do {
            std::vector<std::pair<size_t, size_t>> ballout;
            ballout.reserve(candidates_.size());
            std::transform(candidates_.begin(), candidates_.end(), std::back_inserter(ballout), [&](auto /*name*/) -> std::pair<size_t, size_t> {
                return std::make_pair(0, ballout.size());
            });
            std::for_each(votes_.begin(), votes_.end(), [&](const std::deque<size_t>& vote) {
                if (!vote.empty()) {
                    ballout[vote.front() - 1].first ++;
                }
            });
            std::sort(ballout.begin(), ballout.end(), [](auto c1, auto c2) -> bool {
                return c1.first > c2.first;
            });
            if (ballout[0].first > votes_.size() / 2) {
                winners_.push_back(candidates_[ballout[0].second]);
            } else {
                while (ballout.back().first == 0) {
                    if (std::find(votes_.begin()->begin(), votes_.begin()->end(), ballout.back().second + 1) != votes_.begin()->end()) {
                        break;
                    }
                    ballout.pop_back();
                }
                size_t highest = ballout[0].first;
                bool tie(std::accumulate(ballout.begin(), ballout.end(), true, [&](bool init, auto c) {
                    init &= c.first == highest;
                    return init;
                }));
                if (tie) {
                    std::transform(ballout.begin(), ballout.end(), std::back_inserter(winners_), [&](auto c) -> std::string {
                        return candidates_[c.second];
                    });
                } else {
                    size_t lowest = ballout.back().first;
                    while (ballout.back().first == lowest) {
                        std::for_each(votes_.begin(), votes_.end(), [&](std::deque<size_t>& vote) {
                            std::deque<size_t> temp;
                            std::copy_if(vote.begin(), vote.end(), std::back_inserter(temp), [&] (auto idx) -> bool {
                                return idx - 1 != ballout.back().second;
                            });
                            vote.swap(temp);
                        });
                        ballout.pop_back();
                    }
                }
            }
        } while (winners_.empty());
    } else {
        winners_.assign(candidates_.begin(), candidates_.end());
    }
    return *this;
}

}

#include <fstream>

void U10142::operator()()
{
    solution sol;
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
