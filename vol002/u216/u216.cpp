#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u216.h"

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
    U216 instance;
    instance();
}

namespace {

    class solution {
        std::vector<std::pair<int32_t, int32_t>> locations_;
        uint32_t ncase_;
    public:
        solution() : locations_(), ncase_(0) { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return not locations_.empty();
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.locations_.clear();
        int32_t n;
        in >> n;

        if (n > 0) {
            sol.locations_.reserve(n);
            std::generate_n(std::back_inserter(sol.locations_), n, [&]() {
                int32_t x(0), y(0);
                in >> x >> y;
                return std::make_pair(x, y);
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << "Network #" << sol.ncase_ << std::endl;
        double total (0);

        for (size_t i = 1; i < sol.locations_.size(); i++) {
            auto x (sol.locations_[i].first), y (sol.locations_[i].second);
            auto x1 (sol.locations_[i - 1].first), y1 (sol.locations_[i - 1].second);
            double dist = std::sqrt ((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
            dist += 16;
            out << "Cable requirement to connect (" << x1 << "," << y1 << ") to (" << x << "," << y << ") is " << std::fixed << std::setprecision(
                    2) << dist << " feet." << std::endl;
            total += dist;
        }

        out << "Number of feet of cable required is " << std::fixed << std::setprecision(2) << total << "." << std::endl;
        return out;
    }

    solution& solution::operator()()
    {
        ncase_++;
        double min_distance = std::numeric_limits<double>::max();
        std::vector<std::pair<int32_t, int32_t>> work (locations_.begin(), locations_.end());
        std::vector<int32_t> permutations ({0, 1, 2, 3, 4, 5, 6, 7});
        std::vector<int32_t> temp (locations_.size());

        do {
            for (size_t i = 0; i < locations_.size(); i++) {
                locations_[i] = work[permutations[i]];
            }

            double calculated_distance (0);

            for (size_t i = 1; i < locations_.size(); i++) {
                auto x (locations_[i].first), y (locations_[i].second);
                auto x1 (locations_[i - 1].first), y1 (locations_[i - 1].second);
                calculated_distance += std::sqrt ((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
            }

            if (calculated_distance < min_distance) {
                min_distance = calculated_distance;
                temp.assign(permutations.begin(), permutations.end());
            }
        } while (std::next_permutation(permutations.begin(), permutations.begin() + locations_.size()));

        for (size_t i = 0; i < locations_.size(); i++) {
            locations_[i] = work[temp[i]];
        }

        return *this;
    }

}

void U216::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
