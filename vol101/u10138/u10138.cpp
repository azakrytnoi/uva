#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10138.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10138 instance;
    instance();
}

namespace {
    struct trip_history
    {
        uint16_t day_, hour_, minute_;
        int16_t distance_;
        bool enter_;
    };

    class solution {
    public:
        solution() : trips_(), fares_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool();

    private:
        std::map<std::string, std::pair<double, std::vector<trip_history>>> trips_;
        std::vector<uint16_t> fares_;
    };

    std::istream& operator >> (std::istream& in, trip_history& trip)
    {
        char buff;
        uint16_t month;
        std::string stat;
        in >> month >> buff >> trip.day_ >> buff >> trip.hour_ >> buff >> trip.minute_ >> stat >> trip.distance_;
        trip.enter_ = (stat == "enter");
        return in;
    }

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.fares_.clear();
        sol.trips_.clear();
        sol.fares_.reserve(24);

        if (in)
        {
            std::generate_n(std::back_inserter(sol.fares_), 24, [&]()
            {
                uint16_t tmp;
                in >> tmp;
                return tmp;
            });
            std::string line;
            std::getline(in, line);

            while (std::getline(in, line) && !line.empty())
            {
                std::stringstream iss(line);
                std::string license;
                trip_history trip;
                iss >> license >> trip;
                sol.trips_[license].second.push_back(trip);
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        std::for_each(sol.trips_.begin(), sol.trips_.end(), [&](const std::pair<std::string, std::pair<double, std::vector<trip_history>>>& trips)
        {
            if (trips.second.first != 0)
            {
                double cost(trips.second.first + 2);
                out << trips.first << " $" << std::setprecision(2) << std::fixed << cost << std::endl;
            }
        });
        return out;
    }

    solution::operator bool()
    {
        if (fares_.empty())
        {
            return false;
        }

        solution* self(this);

        for (auto trip = trips_.begin(); trip != trips_.end(); ++trip)
        {
            trip->second.first = 0;
            std::sort(trip->second.second.begin(), trip->second.second.end(), [](trip_history & l, trip_history & r)
            {
                return l.day_ * 24 * 60 + l.hour_ * 60 + l.minute_ < r.day_ * 24 * 60 + r.hour_ * 60 + r.minute_;
            });

            for (size_t idx = 1; idx < trip->second.second.size(); idx++)
            {
                if (trip->second.second[idx - 1].enter_ && !trip->second.second[idx].enter_)
                {
                    int16_t traveled = std::abs(trip->second.second[idx - 1].distance_ - trip->second.second[idx].distance_);
                    double cost = (self->fares_[trip->second.second[idx - 1].hour_] * traveled) / 100.0;
                    trip->second.first += cost + 1;
                }
            }
        }

        return true;
    }
}

void U10138::operator()() const
{
    int N;
    std::cin >> N;
    solution sol;

    while (std::cin >> sol || sol)
    {
        std::cout << sol << std::endl;
    }
}
