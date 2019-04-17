#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u209.h"

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
    U209 instance;
    instance();
}

namespace {
    class point_t {
    public:
        point_t(double_t x = 0, double_t y = 0) : x_(x), y_(y) {}

        double_t x() const
        {
            return x_;
        }
        double_t y() const
        {
            return y_;
        }

        friend bool operator < (const point_t& lhs, const point_t& rhs)
        {
            if (std::abs(lhs.x_ - rhs.x_) > 1e-6)
            {
                return lhs.x_ < rhs.x_;
            }

            return lhs.y_ < rhs.y_;
        }

        static double_t cross(const point_t& zero, const point_t& pta, const point_t& ptb)
        {
            return (pta.x_ - ptb.x_) * (ptb.y_ - zero.y_) - (pta.y_ - zero.y_) * (pta.x_ - zero.x_);
        }

        static double_t distance2(const point_t& pta, const point_t& ptb)
        {
            return (pta.x_ - ptb.x_) * (pta.x_ - ptb.x_) + (pta.y_ - ptb.y_) * (pta.y_ - ptb.y_);
        }

        friend std::istream& operator>>(std::istream& in, point_t& point)
        {
            in >> point.x_ >> point.y_;
            return in;
        }

        //private:
        double_t x_;
        double_t y_;
    };

    enum class shape_t
    {
        none, trinagle, parallelogram, hexagon
    };

    class solution_t {
    public:
        solution_t() : triangles_(), original_(), points_(), shape_(shape_t::none)
        {
            triangles_.resize(std::numeric_limits<int16_t>::max());
            triangles_[1] = {0, 0};
            const auto dy = std::sqrt(3.0) / 2.0;

            for (size_t i = 2, idx = 2; idx < std::numeric_limits<int16_t>::max(); i++)
            {
                triangles_[idx].x_ = triangles_[idx - i + 1].x_ - 0.5;
                triangles_[idx].y_ = triangles_[idx - i + 1].y_ - dy;
                size_t j;

                for (j = 1, idx++; j < i && idx < std::numeric_limits<int16_t>::max(); j++, idx++)
                {
                    triangles_[idx].x_ = triangles_[idx - 1].x_ + 1;
                    triangles_[idx].y_ = triangles_[idx - 1].y_;
                }
            }
        }

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        std::vector<point_t> triangles_;
        std::string original_;
        std::vector<point_t> points_;
        shape_t shape_;

        size_t convex(std::vector<point_t>& points);
    };

    size_t solution_t::convex(std::vector<point_t>& hull)
    {
        std::sort(points_.begin(), points_.end());
        hull.resize(points_.size() * 2);
        int32_t m(0), n(points_.size()), i, t;

        for (i = 0; i < n; i++)
        {
            while (m >= 2 && point_t::cross(hull[m - 2], hull[m - 1], points_[i]) <= 0)
            {
                m--;
            }

            hull[m++] = points_[i];
        }

        for (i = n - 1, t = m + 1; i >= 0; i--)
        {
            while (m >= t && point_t::cross(hull[m - 2], hull[m - 1], points_[i]) <= 0)
            {
                m--;
            }

            hull[m++] = points_[i];
        }

        hull.resize(m);
        return m;
    }

    solution_t& solution_t::operator()()
    {
        if (points_.size() > 1)
        {
//            std::sort(points_.begin(), points_.end());
            std::vector<point_t> points;
            points.reserve(points_.size() * 10);
            size_t metrix = convex(points);

            if (metrix - 1 == points_.size() && metrix - 1 != 5)
            {
                double_t side = point_t::distance2(points[0], points[1]);
                size_t idx;

                for (idx = 1; idx < metrix; idx++)
                {
                    if (std::abs(point_t::distance2(points[idx], points[idx - 1]) - side) > 1e-7)
                    {
                        return *this;;
                    }
                }

//                for (idx = 1; idx < metrix - 1; idx++) {
//                    side = std::abs(point_t::cross(points[idx], points[idx - 1], points[idx + 1]));
//                    auto angle = std::asin(side / std::sqrt(point_t::distance2(points[idx], points[idx - 1])) //
//                                           / std::sqrt(point_t::distance2(points[idx], points[idx + 1]))) * 180 / M_PI;
//
//                    if (std::abs(angle - 60) > 1e-7) {
//                        return *this;
//                    }
//                }

                switch (points_.size())
                {
                case 3:
                    shape_ = shape_t::trinagle;
                    break;

                case 4:
                    shape_ = shape_t::parallelogram;
                    break;

                case 6:
                    shape_ = shape_t::hexagon;
                    break;

                default:
                    break;
                }
            }
        }

        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.points_.clear();
        sol.points_.reserve(6);
        sol.original_.clear();
        sol.shape_ = shape_t::none;

        if (std::getline(in, sol.original_))
        {
            int32_t idx;
            std::stringstream sstream(sol.original_);

            while (sstream >> idx)
            {
                point_t point = sol.triangles_[idx];
                sol.points_.push_back(point);
            }
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << sol.original_ << " ";

        if (sol.shape_ == shape_t::none)
        {
            out << "are not the vertices of an acceptable figure";
        }
        else
        {
            out << "are the vertives of a ";

            switch (sol.shape_)
            {
            case shape_t::trinagle:
                out << "triangle";
                break;

            case shape_t::parallelogram:
                out << "parallelogram";
                break;

            case shape_t::hexagon:
                out << "hexagon";
                break;

            default:
                break;
            }
        }

        return out;
    }
}

void U209::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
