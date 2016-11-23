#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10187.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10187 instance;
    instance();
}

namespace {

    constexpr size_t MAX_CITIES = 100;
    class solution {
        struct vertex;

        struct edge {
            edge() : to_(nullptr), departure_(0), arrival_(0) {}

            edge(const edge& rhs) : to_(rhs.to_), departure_(rhs.departure_), arrival_(rhs.arrival_) {}
            edge& operator=(const edge& rhs)
            {
                to_ = rhs.to_;
                departure_ = rhs.departure_;
                arrival_ = rhs.arrival_;
                return *this;
            }

            vertex* to_;
            int16_t departure_;
            int16_t arrival_;
        };

        struct vertex {
            vertex() : connections_(), cost_(std::numeric_limits<int32_t>::max()), time_(0), prev_(nullptr)
            {
                connections_.reserve(MAX_CITIES);
            }

            vertex(const vertex& rhs) : connections_(rhs.connections_), cost_(rhs.cost_), time_(rhs.time_), prev_(rhs.prev_) {}
            vertex& operator = (const vertex& rhs)
            {
                connections_.assign(rhs.connections_.begin(), rhs.connections_.end());
                cost_ = rhs.cost_;
                time_ = rhs.time_;
                prev_ = rhs.prev_;
                return *this;
            }

            std::vector<edge> connections_;
            int32_t cost_;
            int32_t time_;
            vertex* prev_;
        };

        std::map<std::string, vertex> cities_;
        std::string from_, to_;
        bool solved_;
    public:
        solution() : cities_(), from_(), to_(), solved_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }

        solution& operator()();

    private:
        bool pathExist();
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.cities_.clear();
        size_t n;
        in >> n;

        while (n--) {
            std::string from, to;
            int32_t dep, length;
            in >> from >> to >> dep >> length;

            sol.cities_.insert(std::make_pair(from, solution::vertex()));
            sol.cities_.insert(std::make_pair(to, solution::vertex()));

            int32_t departure ((dep + 12) % 24);
            int32_t arrival (departure + length);

            if ((length <= 12) && (departure >= 6 && departure <= 18) && (arrival >= 6 && arrival <= 18)) {
                solution::edge e;
                e.to_ = &sol.cities_[to];
                e.departure_ = departure;
                e.arrival_ = arrival;
                sol.cities_[from].connections_.push_back(e);
            }
        }

        in >> sol.from_ >> sol.to_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.solved_) {
            out << "Vladimir needs " << sol.cities_.find(sol.to_)->second.cost_ << " litre(s) of blood.";
        } else {
            out << "There is no route Vladimir can take.";
        }

        return out;
    }

    solution& solution::operator()()
    {
        solved_ = cities_.find(from_) != cities_.end() && cities_.find(to_) != cities_.end() && pathExist();
        return *this;
    }

    bool solution::pathExist()
    {
        vertex* source = &cities_[from_];
        vertex* destination = &cities_[to_];
        typedef std::pair<int32_t, vertex*> element;

        std::priority_queue<element, std::vector<element>, std::greater<element>> workQ;
        source->cost_ = 0;
        workQ.push(std::make_pair(0, source));

        while (!workQ.empty()) {
            element ce = workQ.top();
            vertex* current = ce.second;
            workQ.pop();

            if (ce.first != current->cost_) {
                continue;
            }

            if (current == destination) {
                return true;
            }

            for (auto it = current->connections_.begin(); it != current->connections_.end(); ++it) {
                int newCost = current->cost_;

                if (current->time_ > it->departure_) {
                    newCost++;
                }

                if (newCost < it->to_->cost_) {
                    it->to_->cost_ = newCost;
                    it->to_->prev_ = current;
                    it->to_->time_ = it->arrival_;
                    workQ.push(std::make_pair(newCost, it->to_));
                }
            }
        }

        return false;
    }

}

void U10187::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;
    int32_t caseNo(0);

    while (N--) {
        std::cin >> sol;
        std::cout << "Test Case " << (++caseNo) << '.' << std::endl << sol() << std::endl;
    }
}
