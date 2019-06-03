#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10065.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

#include "point.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10065 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_case_(0), points_(), tile_(), cont_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not points_.empty();
        }
        solution_t& operator()();

    private:
        size_t n_case_;
        std::vector<geom::point_t> points_;
        double_t tile_, cont_;
    };

    std::istream& operator >> (std::istream& in, geom::point_t& point)
    {
        in >> point.x_ >> point.y_;
        return in;
    }

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        size_t n(0);
        sol.points_.clear();

        if (in >> n && n > 0)
        {
            sol.points_.reserve(n + 1);
            std::generate_n(std::back_inserter(sol.points_), n, [&]()
            {
                geom::point_t point;
                in >> point;
                return point;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Tile #" << sol.n_case_ << std::endl;
        out << "Wasted Space = " << std::fixed << std::setprecision(2)
            << ((sol.cont_ - sol.tile_) * 100.0 / sol.cont_) << " %" << std::endl;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        ++n_case_;
        auto area = [](std::vector<geom::point_t> points) -> double_t
        {
            points.push_back(points.front());
            double_t res(0);

            for (auto point = points.begin(); point != points.end() - 1; ++point)
            {
                res += point->x_ * (point + 1)->y_ - point->y_ * (point + 1)->x_;
            }

            return std::abs(res / 2.0);
        };
        tile_ = area(points_);
        std::vector<geom::point_t> hull(points_);
        geom::point_util::convexHull(hull);
        cont_ = area(hull);
        return *this;
    }

}

void U10065::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
