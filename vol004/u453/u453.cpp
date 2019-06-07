#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u453.h"

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
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U453 instance;
    instance();
}

namespace {

    const double_t EPS = 1e-6;

    struct circle_t
    {
        double_t x_, y_, r_;

        circle_t() : x_(), y_(), r_() {}

        friend std::istream& operator>>(std::istream& in, circle_t& circle)
        {
            in >> circle.x_ >> circle.y_ >> circle.r_;
            return in;
        }
    };

    struct point_t
    {
        double_t x_, y_;

        point_t(double_t x = 0, double_t y = 0) : x_(x), y_(y) {}

        bool operator<(const point_t& rhs) const
        {
            if (std::abs(x_ - rhs.x_) < EPS)
            {
                return y_ < rhs.y_;
            }

            return x_ < rhs.x_;
        }

        friend std::ostream& operator<<(std::ostream& out, const point_t& point)
        {
            auto round = [](const double_t val) -> double_t
            {
                return std::abs(val) < 1e-4 ? 0.0 : val;
            };
            out << "(" << std::fixed << std::setprecision(3) << round(point.x_) << "," << round(point.y_) << ")";
            return out;
        }
    };

    class solution_t {
    public:
        solution_t() : a_(), b_(), points_(), n_points_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        circle_t a_, b_;
        std::vector<point_t> points_;
        int64_t n_points_;

        int64_t intersect();
    };

    solution_t& solution_t::operator()()
    {
        points_.clear();
        points_.reserve(2);
        n_points_ = intersect();
        std::sort(points_.begin(), points_.end());
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        in >> sol.a_ >> sol.b_;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        if (sol.n_points_ < 0)
        {
            out << "NO INTERSECTION";
        }
        else if (sol.n_points_ == 0)
        {
            out << "THE CIRCLES ARE THE SAME";
        }
        else
        {
            std::ostream_iterator<point_t> pout(out, "");
            std::copy(sol.points_.begin(), sol.points_.end(), pout);
        }

        return out;
    }

    int64_t solution_t::intersect()
    {
        double_t distanceAB = std::sqrt((a_.x_ - b_.x_) * (a_.x_ - b_.x_) + (a_.y_ - b_.y_) * (a_.y_ - b_.y_));

        // shortcut for circles with same centers
        if (std::abs(distanceAB) < EPS && std::abs(a_.r_ - b_.r_) < EPS)
        {
            if (a_.r_ < EPS)
            {
                points_.push_back(point_t(a_.x_, a_.y_));
            }

            return points_.size();
        }

        // no intersection in any combination
        if (a_.r_ + b_.r_ + EPS < distanceAB || a_.r_ + distanceAB + EPS < b_.r_ || b_.r_ + distanceAB + EPS < a_.r_)
        {
            return -1;
        }

        double_t dx = b_.x_ - a_.x_;
        double_t dy = b_.y_ - a_.y_;

        if (std::abs(distanceAB - a_.r_ - b_.r_) < EPS || std::abs(a_.r_ - distanceAB - b_.r_) < EPS ||
                std::abs(b_.r_ - a_.r_ - distanceAB) < EPS)   // circles touch in single point
        {
            if (std::abs(distanceAB - a_.r_ - b_.r_) < EPS)   // (A)(B)
            {
                points_.push_back(point_t(a_.x_ + dx * a_.r_ / (a_.r_ + b_.r_), a_.y_ + dy * a_.r_ / (a_.r_ + b_.r_)));
            }
            else if (a_.r_ < b_.r_)     //((A)B)
            {
                points_.push_back(point_t(a_.x_ - dx * a_.r_ / (b_.r_ - a_.r_), a_.y_ - dy * a_.r_ / (b_.r_ - a_.r_)));
            }
            else     //((B)A)
            {
                points_.push_back(point_t(b_.x_ + dx * b_.r_ / (a_.r_ - b_.r_), b_.y_ + dy * b_.r_ / (a_.r_ - b_.r_)));
            }
        }
        else     // 2 intersection point
        {
            double_t theta = acos((a_.r_ * a_.r_ + distanceAB * distanceAB - b_.r_ * b_.r_) / 2 / a_.r_ / distanceAB);
            double_t r_dx = dx * cos(theta) - dy * sin(theta);
            double_t r_dy = dx * sin(theta) + dy * cos(theta);
            points_.push_back(point_t(a_.x_ + r_dx * a_.r_ / distanceAB, a_.y_ + r_dy * a_.r_ / distanceAB));
            r_dx = dx * cos(-theta) - dy * sin(-theta);
            r_dy = dx * sin(-theta) + dy * cos(-theta);
            points_.push_back(point_t(a_.x_ + r_dx * a_.r_ / distanceAB, a_.y_ + r_dy * a_.r_ / distanceAB));
        }

        return points_.size();
    }

}

void U453::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
