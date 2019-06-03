#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u428.h"

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
    U428 instance;
    instance();
}

namespace {

    class lot_t : public std::tuple<double_t, double_t, double_t, double_t> {
    public:
        friend
        std::istream& operator>>(std::istream& in, lot_t& t)
        {
            in >> std::get<0>(t) >> std::get<1>(t) >> std::get<2>(t) >> std::get<3>(t);
            return in;
        }

    };

    class solution_t {
    public:
        solution_t() : total_roof_(), total_floor_(), total_lot_(), lot_roof_(), lot_floor_(), lot_(), n_lots_(), lot_det_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();
        void totals(std::ostream& out);

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        double_t total_roof_, total_floor_, total_lot_;
        double_t lot_roof_, lot_floor_, lot_;
        size_t n_lots_;
        std::vector<lot_t> lot_det_;
    };

    solution_t& solution_t::operator()()
    {
        ++n_lots_;
        lot_roof_ = lot_floor_ = 0;

        for (auto& lot_det : lot_det_)
        {
            double_t base = (std::get<0>(lot_det) + std::get<1>(lot_det)) * std::get<2>(lot_det) / 2.0;
            lot_roof_ += base;
            lot_floor_ += base * std::cos(std::get<3>(lot_det) * M_PI / 180.0);
        }

        total_roof_ += lot_roof_;
        total_floor_ += lot_floor_;
        total_lot_ += lot_;
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.lot_det_.clear();
        std::string lot;
        std::string line;

        while (std::getline(in, line) && not line.empty())
        {
            lot += " " + line;
        }

        if (not lot.empty())
        {
            std::stringstream sin(lot);
            sin >> sol.lot_;
            std::istream_iterator<lot_t> slot(sin);
            std::copy(slot, std::istream_iterator<lot_t>(), std::back_inserter(sol.lot_det_));
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        if (sol.n_lots_ == 1)
        {
            out << "Roof Area" << std::string(5, ' ') << "Floor Area" << std::string(5, ' ') << "% Covered" << std::endl //
                << "---------" << std::string(5, ' ') << "----------" << std::string(5, ' ') << "---------" << std::endl;
        }

        out << std::right << std::fixed << std::setprecision(2) << std::setw(9) << sol.lot_roof_ << std::string(5, ' ') //
            << std::right << std::fixed << std::setprecision(2) << std::setw(10) << sol.lot_floor_ << std::string(5, ' ')  //
            << std::right << std::fixed << std::setprecision(2) << std::setw(8) << (sol.lot_floor_ * 100.0 / sol.lot_) << "%";
        return out;
    }

    void solution_t::totals(std::ostream& out)
    {
        out << std::endl //
            << "Total surface area of roofs" //
            << std::fixed << std::right << std::setprecision(2) << std::setw(12) << total_roof_ << std::endl //
            << "Total area covered by roofs"//
            << std::fixed << std::right << std::setprecision(2) << std::setw(12) << total_floor_ << std::endl //
            << "Percentage of total area covered by roofs" //
            << std::fixed << std::right << std::setprecision(2) << std::setw(8) << (total_floor_ * 100.0 / total_lot_) << "%" << std::endl //
            << "Average roof surface area per lot" //
            << std::fixed << std::right << std::setprecision(2) << std::setw(16) << (total_roof_ / n_lots_) << std::endl //
            << "Average floor space covered per lot" //
            << std::fixed << std::right << std::setprecision(2) << std::setw(14) << (total_floor_ / n_lots_) << std::endl;
    }

}

void U428::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }

    sol.totals(std::cout);
}
