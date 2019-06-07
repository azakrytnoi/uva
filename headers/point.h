/*
 * point.h
 *
 *  Created on: Mar 9, 2019
 *      Author: aleksey
 */

#pragma once

#include <cmath>
#include <vector>
#include <algorithm>

namespace geom {
    const double_t EPS = 1e-6;

    class point_t {
    public:
        double_t x_, y_;

        point_t(double_t x = 0, double_t y = 0) : x_(x), y_(y) {}

        static double_t cross(const point_t& o, const point_t& a, const point_t& b)
        {
            return (a.x_ - o.x_) * (b.y_ - o.y_) - (a.y_ - o.y_) * (b.x_ - o.x_);
        }

        static double_t distance(const point_t& a, const point_t& b)
        {
            return std::sqrt((a.x_ - b.x_) * (a.x_ - b.x_) + (a.y_ - b.y_) * (a.y_ - b.y_));
        }

        bool operator<(const point_t& rhs) const
        {
            if (std::abs(x_ - rhs.x_) < geom::EPS)
            {
                return y_ < rhs.y_;
            }

            return x_ < rhs.x_;
        }

        bool operator==(const point_t& rhs) const
        {
            return std::abs(x_ - rhs.x_) < geom::EPS && std::abs(y_ - rhs.y_) < geom::EPS;
        }

        friend point_t operator+(const point_t& lhs, const point_t& rhs)
        {
            return point_t(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);
        }

        friend point_t operator-(const point_t& lhs, const point_t& rhs)
        {
            return point_t(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_);
        }

        point_t& operator+=(const point_t& rhs)
        {
            x_ += rhs.x_, y_ += rhs.y_;
            return *this;
        }

        point_t& operator-=(const point_t& rhs)
        {
            x_ -= rhs.x_, y_ -= rhs.y_;
            return *this;
        }

        double_t operator^(const point_t& rhs) const
        {
            return x_ * rhs.y_ - y_ * rhs.x_;
        }

        point_t& operator /= (double_t r)
        {
            x_ /= r, y_ /= r;
            return *this;
        }
    };

    class point_util {
    public:
        static void convexHull(std::vector<point_t>& points)
        {
            size_t n = points.size();
            std::vector<point_t> convex_hull(2 * n);

            std::sort(points.begin(), points.end());
            points.erase(std::unique(points.begin(), points.end()), points.end());

            size_t k = 0;

            for (size_t i = 0; i < n; i++)
            {
                while (k >= 2 && point_t::cross(convex_hull[k - 2],
                                                convex_hull[k - 1],
                                                points[i]) <= geom::EPS)
                {
                    --k;
                }

                convex_hull[k++] = points[i];
            }

            size_t t = k + 1;

            for (int64_t i = n - 2; i >= 0; i--)
            {
                while (k >= t && point_t::cross(convex_hull[k - 2],
                                                convex_hull[k - 1],
                                                points[i]) <= geom::EPS)
                {
                    --k;
                }

                convex_hull[k++] = points[i];
            }

            points.assign(convex_hull.begin(), convex_hull.begin() + k - 1);
        }
    };
}  // namespace geom
