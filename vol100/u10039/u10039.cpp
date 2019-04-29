#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10039.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <cstring>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10039 instance;
    instance();
}

namespace {

    struct edge_t
    {
        size_t to_;
        int16_t start_time_, end_time_;
        edge_t(size_t to = 0, int16_t start = 0, int16_t end = 0) : to_(to), start_time_(start), end_time_(end) {}
    };

    struct result_t
    {
        bool valid_;
        uint16_t departure_time_, arrival_time_;
        std::string departure_, arrival_;

        result_t() : valid_(false), departure_time_(0), arrival_time_(0), departure_(), arrival_() {}

        operator bool() const
        {
            return valid_;
        }

        friend std::ostream& operator<< (std::ostream& out, const result_t& result)
        {
            if (result.valid_)
            {
                out << "Departure " << std::setw(4) << std::setfill('0') << result.departure_time_ << ' ' << result.departure_ << std::endl
                    << "Arrival   " << std::setw(4) << std::setfill('0') << result.arrival_time_ << ' ' << result.arrival_ << std::endl;
            }
            else
            {
                out << "No connection" << std::endl;
            }

            return out;
        }
    };

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), Case_No_(0), cities_(), edges_()
            , start_trip_(), start_(), end_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_, Case_No_;
        std::map<std::string, size_t> cities_;
        std::vector<std::vector<edge_t>> edges_;
        uint16_t start_trip_;
        std::string start_, end_;
        result_t result_;
    };

    template<typename _Container>
    class map_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
    private:
        _Container* container_;

    public:
        typedef _Container          container_type;

        explicit
        map_insert_iterator(_Container& __x)
            : container_(std::__addressof(__x)) { }

        map_insert_iterator& operator=(const typename _Container::key_type& __key)
        {
            container_->insert(std::make_pair(__key, container_->size()));
            return *this;
        }

        map_insert_iterator& operator=(typename _Container::key_type&& __key)
        {
            container_->push_back(std::make_pair(std::move(__key), container_->size()));
            return *this;
        }

        map_insert_iterator& operator*()
        {
            return *this;
        }

        map_insert_iterator& operator++()
        {
            return *this;
        }

        map_insert_iterator operator++(int)
        {
            return *this;
        }
    };

    template<typename _Container>
    inline map_insert_iterator<_Container>
    map_inserter(_Container& __x)
    {
        return map_insert_iterator<_Container>(__x);
    }

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        sol.cities_.clear();
        sol.edges_.clear();
        size_t n_cities(0);

        if (in >> n_cities)
        {
            std::istream_iterator<std::string> city_in(in);
            std::copy_n(city_in, n_cities, map_inserter(sol.cities_));
            sol.edges_.resize(n_cities);
            size_t n_trains(0);
            in >> n_trains;
            size_t from(0), to(0);
            uint16_t start_time(0), end_time(0);
            std::string name;

            while (n_trains--)
            {
                size_t n(0);
                in >> n;
                bool first(true);

                while (n--)
                {
                    in >> end_time >> name;
                    to = sol.cities_[name];

                    if (not first && end_time >= start_time)
                    {
                        sol.edges_[from].push_back(edge_t(to, start_time, end_time));
                    }

                    from = to, start_time = end_time;
                    first = false;
                }
            }

            in >> sol.start_trip_ >> sol.start_ >> sol.end_;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Scenario " << sol.Case_No_ << std::endl;
        out << sol.result_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        result_ = result_t();
        size_t from(cities_[start_]), to(cities_[end_]);
        ++Case_No_;
        std::vector<std::vector<int16_t>> timetable(2400, std::vector<int16_t>(edges_.size() + 1, -1));

        for (size_t i = 0; i < edges_[from].size(); ++i)
        {
            if (edges_[from][i].start_time_ >= start_trip_)
            {
                timetable[edges_[from][i].end_time_][edges_[from][i].to_] = std::max(timetable[edges_[from][i].end_time_][edges_[from][i].to_],
                        edges_[from][i].start_time_);
            }
        }

        for (int16_t i = start_trip_; i < 2400; ++i)
        {
            for (size_t j = 0; j < cities_.size(); ++j)
            {
                if (timetable[i][j] == -1)
                {
                    continue;
                }

                for (size_t k = 0; k < edges_[j].size(); ++k)
                {
                    if (edges_[j][k].start_time_ >= i)
                    {
                        timetable[edges_[j][k].end_time_][edges_[j][k].to_] = std::max(timetable[edges_[j][k].end_time_][edges_[j][k].to_], timetable[i][j]);
                    }
                }
            }

            if (timetable[i][to] != -1)
            {
                result_.valid_ = true;
                result_.departure_time_ = timetable[i][to];
                result_.arrival_time_ = i;
                result_.departure_ = start_;
                result_.arrival_ = end_;
                break;
            }
        }

        return *this;
    }

}

void U10039::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
