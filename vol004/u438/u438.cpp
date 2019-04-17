#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u438.h"

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
    U438 instance;
    instance();
}

namespace {

    class point_t {
    public:
        double_t x_, y_;

        point_t() : x_(), y_() {}

        static double_t cross(const point_t& o, const point_t& a, const point_t& b)
        {
            return (a.x_ - o.x_) * (b.y_ - o.y_) - (a.y_ - o.y_) * (b.x_ - o.x_);
        }

        static double_t distance(const point_t& a, const point_t& b)
        {
            return std::sqrt((a.x_ - b.x_) * (a.x_ - b.x_) + (a.y_ - b.y_) * (a.y_ - b.y_));
        }

        friend std::istream& operator >>(std::istream& in, point_t& point)
        {
            in >> point.x_ >> point.y_;
            return in;
        }
    };
    class solution_t {
    public:
        solution_t() : a_(), b_(), c_(), circ_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        point_t a_, b_, c_;
        double_t circ_;
    };

    solution_t& solution_t::operator()()
    {
        auto area(std::abs(point_t::cross(a_, b_, c_)) / 2.0);
        auto r(point_t::distance(a_, b_)*point_t::distance(b_, c_)*point_t::distance(c_, a_) / 4.0 / area);
        circ_ = 2.0 * M_PI * r;
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        in >> sol.a_ >> sol.b_ >> sol.c_;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << std::fixed << std::setprecision(2) << sol.circ_;
        return out;
    }

}

void U438::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
