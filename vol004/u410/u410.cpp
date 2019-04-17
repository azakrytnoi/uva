#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u410.h"

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
    U410 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), n_chambers_(), imbalance_(), spec_(), chamber_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t case_no_;
        uint16_t n_chambers_;
        double_t imbalance_;
        std::vector<uint16_t> spec_;
        std::vector<std::pair<double_t, std::vector<uint16_t>>> chamber_;
    };

    solution_t& solution_t::operator()()
    {
        case_no_++;
        chamber_.resize(n_chambers_);
        std::sort(spec_.begin(), spec_.end());
        double_t avg = std::accumulate(spec_.begin(), spec_.end(), 0.0);
        avg /= n_chambers_;

        auto spec_i = spec_.rbegin();
        auto chamber_i = chamber_.begin();

        for (int16_t direction = 1; spec_i != spec_.rend(); ++spec_i, std::advance(chamber_i, direction))
        {
            chamber_i->first += *spec_i;
            chamber_i->second.push_back(*spec_i);

            if (chamber_i == chamber_.end() - 1)
            {
                --direction;
            }
        }

        imbalance_ = std::accumulate(chamber_.begin(), chamber_.end(), 0.0, [&](const double_t prev,
                                     const std::pair<double_t, std::vector<uint16_t>>& val)
        {
            return prev + std::abs(val.first - avg);
        });
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.chamber_.clear();
        sol.spec_.clear();

        if (in >> sol.n_chambers_)
        {
            size_t n(0);
            in >> n;
            std::istream_iterator<uint16_t> sin(in);
            sol.spec_.reserve(n);
            std::copy_n(sin, n, std::back_inserter(sol.spec_));
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << "Set #" << sol.case_no_ << std::endl;

        for (size_t idx = 0; idx < sol.chamber_.size(); ++idx)
        {
            out << std::right << std::setw(2) << idx << ':';
            std::for_each(sol.chamber_[idx].second.begin(), sol.chamber_[idx].second.end(), [&](const uint16_t val)
            {
                out << " " << val;
            });
            out << std::endl;
        }

        out << "IMBALANCE = " << std::fixed << std::setprecision(5) << sol.imbalance_ << std::endl;
        return out;
    }

}

void U410::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
