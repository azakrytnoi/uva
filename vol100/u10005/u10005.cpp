#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10005.h"

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
#include <complex>

#include "point.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10005 instance;
    instance();
}

namespace {

    struct point_t : public std::complex<double_t>
    {
        point_t(double_t r = 0, double_t i = 0) : std::complex<double_t>(r, i) {}
        point_t(const point_t& p) : std::complex<double_t>(p.real(), p.imag()) {}
        point_t(std::complex<double_t> p) : std::complex<double_t>(p.real(), p.imag()) {}

        friend std::istream& operator >>(std::istream& in, point_t& point);
    };

    struct circle_t
    {
        const point_t center_;
        double_t radius_;

        circle_t (const point_t& point, double_t radius) : center_(std::move(point)), radius_(radius) {}
    };

    class solution_t {
    public:
        solution_t() : points_(), radius_(), max_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not points_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<point_t> points_;
        double_t radius_;
        size_t max_;

        void line_circ_inter(const circle_t& a, const circle_t& b, std::vector<point_t>& inter);
        size_t points_in_circle(const circle_t& c);
        size_t points_contained(point_t& p1, point_t& p2);
    };

    std::istream& operator >>(std::istream& in, point_t& point)
    {
        double_t r(0), i(0);
        in >> r >> i;
        point.real(r), point.imag(i);
        return in;
    }

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.points_.clear();
        size_t n(0);

        if (in >> n && n > 0)
        {
            sol.points_.reserve(n);
            std::istream_iterator<point_t> pin(in);
            std::copy_n(pin, n, std::back_inserter(sol.points_));
            in >> sol.radius_;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << (sol.max_ == sol.points_.size() ? "The polygon can be packed in the circle." : "There is no way of packing that polygon.");
        return out;
    }

    solution_t& solution_t::operator()()
    {
        max_ = 0;

        for (auto i = points_.begin(); i != points_.end(); ++i)
        {
            for (auto j = i + 1; j != points_.end(); ++j)
            {
                max_ = std::max(max_, points_contained(*i, *j));
            }
        }

        return *this;
    }

    void solution_t::line_circ_inter(const circle_t& a, const circle_t& b, std::vector<point_t>& inter)
    {
        double_t d2 (std::norm(b.center_ - a.center_)), rS (a.radius_ + b.radius_), rD (a.radius_ - b.radius_);

        if (d2 > rS * rS || d2 < rD * rD)
        {
            return;
        }

        double_t ca (0.5 * (1 + rS * rD / d2));
        point_t z (ca, std::sqrt((a.radius_ * a.radius_ / d2) - ca * ca));
        inter.push_back(a.center_ + (b.center_ - a.center_) * z);

        if (fabs(z.imag()) > geom::EPS)
        {
            inter.push_back(a.center_ + (b.center_ - a.center_) * std::conj(z));
        }
    }

    size_t solution_t::points_in_circle(const circle_t& c)
    {
        size_t count (std::accumulate(points_.begin(), points_.end(), 0, [&](auto prev, const auto & point)
        {
            return std::norm(point - c.center_) <= c.radius_ * c.radius_ ? ++prev : prev;
        }));
        return count;
    }

    size_t solution_t::points_contained(point_t& p1, point_t& p2)
    {
        std::vector<point_t> inter;
        line_circ_inter(circle_t(p1, radius_), circle_t(p2, radius_), inter);

        size_t best (std::accumulate(inter.begin(), inter.end(), 0, [&](size_t prev, const auto & center)
        {
            return std::max(prev, points_in_circle(circle_t(center, radius_)));
        }));
        return best;
    }
}

void U10005::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
