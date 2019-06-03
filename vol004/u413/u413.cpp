#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u413.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U413 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : avg_up_(), avg_down_(), data_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not data_.empty();
        }
        solution_t& operator()();

    private:
        double_t avg_up_, avg_down_;
        std::vector<uint16_t> data_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.data_.clear();
        uint16_t x(0);

        while (in >> x && x > 0)
        {
            sol.data_.push_back(x);
        }

        sol.data_.shrink_to_fit();
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Nr values = " << sol.data_.size() << ":  " << std::fixed << std::setprecision(6) << sol.avg_up_ << " " << sol.avg_down_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        enum class direction_t
        {
            none, up, down
        };
        std::map<direction_t, std::pair<size_t, double_t>> seqences;
        direction_t direction(direction_t::none);

        for (auto idx = data_.begin() + 1; idx != data_.end(); ++idx)
        {
            if (*(idx - 1) < *idx && direction != direction_t::up)
            {
                seqences[direction_t::up].first += seqences[direction_t::none].first;
                seqences[direction_t::none].first = 0;
                direction = direction_t::up;
                seqences[direction].second++;
            }
            else if (*(idx - 1) > *idx && direction != direction_t::down)
            {
                seqences[direction_t::down].first += seqences[direction_t::none].first;
                seqences[direction_t::none].first = 0;
                direction = direction_t::down;
                seqences[direction].second++;
            }

            seqences[direction].first++;
        }

        avg_up_ = seqences[direction_t::up].second == 0 ? 0.0 : seqences[direction_t::up].first / seqences[direction_t::up].second;
        avg_down_ = seqences[direction_t::down].second == 0 ? 0.0 : seqences[direction_t::down].first / seqences[direction_t::down].second;
        return *this;
    }

}

void U413::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
