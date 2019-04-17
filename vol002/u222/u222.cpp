#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u222.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U222 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), distance_(), tank_(), mi_per_gallon_(), costs_(), stations_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return distance_ > 0;
        }
        solution_t& operator()();

    private:
        typedef std::pair<double_t, double_t> station_t;
        typedef std::vector<station_t>::iterator station_ref;
        size_t case_no_;
        double_t distance_, tank_, mi_per_gallon_, costs_;
        std::vector<station_t> stations_;

        double_t optimize (double_t distance, station_ref station);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.stations_.clear();

        if (in >> sol.distance_ && sol.distance_ > 0)
        {
            size_t n_stations(0);
            in >> sol.tank_ >> sol.mi_per_gallon_ >> sol.costs_ >> n_stations;
            sol.stations_.reserve(n_stations + 1);
            std::generate_n(std::back_inserter(sol.stations_), n_stations, [&]()
            {
                double_t dist(0), price(0);
                in >> dist >> price;
                return std::make_pair(dist, price);
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Data Set #" << sol.case_no_ << std::endl << "minimum cost = $" << std::fixed << std::setprecision(2) << sol.costs_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        case_no_++;
        costs_ += optimize(0, stations_.begin()) / 100;
        return *this;
    }

    double_t solution_t::optimize(double_t distance, station_ref station)
    {
        if (distance + tank_ * mi_per_gallon_ >= distance_ || station == stations_.end())
        {
            return 0;
        }

        double_t best = std::numeric_limits<double_t>::max();

        for (station_ref next_station = station; next_station != stations_.end(); ++next_station)
        {
            auto fuel_used = (next_station->first - distance) / mi_per_gallon_;
            auto fuel_remains = tank_ - fuel_used;

            if (fuel_remains < 0)
            {
                break;
            }

            if (fuel_remains <= tank_ / 2.0)
            {
                best = std::min(best, 200 + fuel_used * next_station->second + optimize(next_station->first, next_station + 1));
            }
            else if (fuel_remains > tank_ / 2.0 && next_station + 1 != stations_.end()   //
                     && (next_station + 1)->first > distance + tank_ * mi_per_gallon_)
            {
                return std::floor(200 + fuel_used * next_station->second + optimize(next_station->first, next_station + 1) + 0.5);
            }
        }

        return std::floor(best + 0.5);
    }
}

void U222::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
