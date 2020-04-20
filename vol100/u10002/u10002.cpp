#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10002.h"

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
    U10002 instance;
    instance();
}

namespace {

    typedef geom::point_t point_t;

    class solution_t {
    public:
        solution_t() : points_(), center_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not points_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<point_t> points_;
        point_t center_;

        void computeCenter()
        {
            double_t area = 0.0;
            center_.x_ = center_.y_ = 0.0;

            for (size_t i = 1; i < points_.size() - 1; i++)
            {
                auto tmp = point_t::cross(points_[i], points_[0], points_[i + 1]) / 2.0;
                center_.x_ += (points_[0].x_ + points_[i].x_ + points_[i + 1].x_) * tmp;
                center_.y_ += (points_[0].y_ + points_[i].y_ + points_[i + 1].y_) * tmp;
                area += tmp;
            }

            center_.x_ /= (3 * area);
            center_.y_ /= (3 * area);
        }
    };

    std::ostream& operator<<(std::ostream& out, const point_t& point)
    {
        constexpr size_t precision (3);
        const double_t EPS (std::pow(10, -double_t(precision + 1)));
        auto round = [&](const double_t val) -> double_t
        {
            return std::abs(val) < EPS ? 0.0 : val;
        };
        out << std::fixed << std::setprecision(precision) << round(point.x_) << ' ' << round(point.y_);
        return out;
    }

    std::istream& operator>>(std::istream& in, point_t& point)
    {
        in >> point.x_ >> point.y_;
        return in;
    }

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.points_.clear();
        size_t n(0);

        if (in >> n && n >= 3)
        {
            sol.points_.resize(n);

            for (auto& point : sol.points_)
            {
                in >> point;
            };
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.center_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        geom::point_util::convexHull(points_);
        computeCenter();
        return *this;
    }

}

void U10002::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
