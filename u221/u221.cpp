#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u221.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U221 instance;
    instance();
}

namespace {

    struct building_t {
        int16_t x_, y_;
        int16_t width_, depth_, height_, label_;

        building_t() : x_(), y_(), width_(), depth_(), height_(), label_(-1) {}

        friend std::istream& operator >> (std::istream& in, building_t& building)
        {
            in >> building.x_ >> building.y_ >> building.width_ >> building.depth_ >> building.height_;
            return in;
        }

        bool operator < (const building_t& rhs) const
        {
            auto lhs_coord (std::make_pair(x_, y_));
            auto rhs_coord (std::make_pair(rhs.x_, rhs.y_));

            if (lhs_coord != rhs_coord) {
                return lhs_coord < rhs_coord;
            }

            return depth_ < rhs.depth_;
        }
    };

    class solution_t {
    public:
        solution_t() : n_case_(), buildings_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not buildings_.empty();
        }
        solution_t& operator()();

    private:
        size_t n_case_;
        std::vector<building_t> buildings_;
        std::vector<int16_t> result_;

        bool cover(std::vector<std::pair<int16_t, int16_t>>& interval, int16_t left, int16_t right) const;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.buildings_.clear();
        sol.result_.clear();
        size_t n_builings(0);

        if (in >> n_builings && n_builings > 0) {
            sol.buildings_.reserve(n_builings);
            std::istream_iterator<building_t> iin(in);
            std::copy_n(iin, n_builings, std::back_inserter(sol.buildings_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "For map #" << sol.n_case_ << ", the visible buildings are numbered as follows:" << std::endl;
        std::ostream_iterator<int16_t> iout (out, " ");
        std::copy(sol.result_.begin(), sol.result_.end(), iout);
        out << std::endl;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        n_case_++;
        int16_t label(0);
        std::for_each(buildings_.begin(), buildings_.end(), [&](building_t& b) {
            b.label_ = ++label;
        });
        std::sort(buildings_.begin(), buildings_.end());

        for (auto building_i = buildings_.begin(); building_i != buildings_.end(); ++building_i) {
            std::vector<std::pair<int16_t, int16_t>> interval;

            for (auto building_j = buildings_.begin(); building_j != buildings_.end(); ++building_j) {
                if (building_j == building_i)	{
                    continue;
                }

                if (building_i->height_ > building_j->height_ || building_i->y_ <= building_j->y_) {
                    continue;
                }

                int16_t left = std::max(building_i->x_, building_j->x_), right = std::min(building_i->x_ + building_i->width_, building_j->x_ + building_j->width_);

                if (left < right) {
                    interval.push_back(std::make_pair(left, right));
                }
            }

            if (not cover(interval, building_i->x_, building_i->x_ + building_i->width_)) {
                result_.push_back(building_i->label_);
            }
        }

        return *this;
    }

    bool solution_t::cover(std::vector<std::pair<int16_t, int16_t>>& interval, int16_t left, int16_t right) const
    {
        std::sort(interval.begin(), interval.end());
        int16_t y = left;

        for (size_t i = 0; i < interval.size(); i++) {
            if (interval[i].first <= y) {
                y = std::max(y, interval[i].second);
            } else {
                return 0;
            }
        }

        return y >= right;
    }
}

void U221::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
