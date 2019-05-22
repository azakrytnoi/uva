#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10084.h"

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

#include "point.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10084 instance;
    instance();
}

namespace {

    typedef geom::point_t point_t;
    typedef std::vector<point_t> polygon_t;

    struct line_t
    {
        double_t a_, b_, c_;

        line_t(double_t a = 0, double_t b = 0, double_t c = 0) : a_(a), b_(b), c_(c) {}
        line_t(const point_t& p1, const point_t& p2) :
            a_(p1.x_ - p2.x_), b_(p1.y_ - p2.y_), c_()
        {
            c_ = -(a_ * ((p1.x_ + p2.x_) / 2.0) + b_ * ((p1.y_ + p2.y_) / 2.0));
        }

        double_t operator ()(const point_t& point) const
        {
            return a_ * point.x_ + b_ * point.y_ + c_;
        }
    };

    class solution_t {
    public:
        solution_t() : points_({{0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0}, {0.0, 10.0}}),
        start_({0.0, 0.0}), current_(), hint_(), area_()
        {
            points_.reserve(500);
        }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        polygon_t points_;
        point_t start_, current_;
        std::string hint_;
        double_t area_;

        double_t area() const
        {
            double_t summ(0);

            for (auto p = points_.begin(); p != points_.end(); ++p)
            {
                auto p_next(*p == points_.back() ? points_.begin() : p + 1);
                summ += *p ^ *p_next;
            }

            return std::abs(summ / 2.0);
        }

        std::vector<point_t> build_inter(polygon_t& b_side, polygon_t& c_side, line_t& median);
    };

    std::istream& operator >> (std::istream& in, point_t& point)
    {
        in >> point.x_ >> point.y_;
        return in;
    }

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.current_ >> sol.hint_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << std::fixed << std::setprecision(2) << sol.area_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        if (hint_ == "Same")
        {
            points_.clear();
        }
        else
        {
            line_t median(start_, current_);
            polygon_t b_side, c_side;
            b_side.reserve(500);
            c_side.reserve(500);
            auto inter(build_inter(b_side, c_side, median));

            if (inter.size() == 2)
            {
                bool flag1(median(b_side[b_side.size() / 2]) < geom::EPS);
                point_t x(hint_ == "Hotter" ? current_ : start_);

                bool flag2(median(x) < geom::EPS);

                if (flag1 == flag2)
                {
                    points_.assign(b_side.begin(), b_side.end());
                }
                else
                {
                    points_.assign(c_side.begin(), c_side.end());
                }

                while (std::abs(points_.back().x_ - points_.front().x_) < geom::EPS
                        && std::abs(points_.back().y_ - points_.front().y_) < geom::EPS)
                {
                    points_.erase(points_.end());
                }

            }
        }

        area_ = area();
        start_ = current_;
        return *this;
    }

    std::vector<point_t> solution_t::build_inter(polygon_t& b_side, polygon_t& c_side, line_t& median)
    {
        std::vector<point_t> inter;
        inter.reserve(2);

        for (auto p = points_.begin(); p != points_.end(); ++p)
        {
            auto p_next(p == points_.end() - 1 ? points_.begin() : p + 1);

            if (inter.size() == 1)
            {
                if (b_side.empty() || std::abs(b_side.back().x_ - p->x_) > geom::EPS
                        || std::abs(b_side.back().y_ - p->y_) > geom::EPS)
                {
                    b_side.push_back(*p);
                }
            }
            else
            {
                if (c_side.empty() || std::abs(c_side.back().x_ - p->x_) > geom::EPS
                        || std::abs(c_side.back().y_ - p->y_) > geom::EPS)
                {
                    c_side.push_back(*p);
                }
            }

            if (median(*p) * median(*p_next) <= geom::EPS)
            {
                if (inter.size() < 2)
                {
                    line_t temp;

                    if (std::abs(p->x_ - p_next->x_) < geom::EPS)
                    {
                        temp = {1, 0, -p->x_};
                    }
                    else
                    {
                        double_t tmp((p->y_ - p_next->y_) / (p->x_ - p_next->x_));
                        temp = {tmp, -1, -(p->x_ * tmp - p->y_)};
                    }

                    double_t r((median.a_ * temp.b_ - temp.a_ * median.b_));

                    if (std::abs(r) > geom::EPS)
                    {
                        point_t x((-median.c_)*temp.b_ - (-temp.c_ * median.b_), median.a_ * (-temp.c_) - temp.a_ * (-median.c_));
                        x /= r;

                        if (inter.empty() || std::abs(x.x_ - inter.front().x_) > geom::EPS || std::abs(x.y_ - inter.front().y_) > geom::EPS)
                        {
                            inter.push_back(x);

                            if (b_side.empty() || std::abs(b_side.back().x_ - x.x_) > geom::EPS || std::abs(b_side.back().y_ - x.y_) > geom::EPS)
                            {
                                b_side.push_back(x);
                            }

                            if (c_side.empty() || std::abs(c_side.back().x_ - x.x_) > geom::EPS || std::abs(c_side.back().y_ - x.y_) > geom::EPS)
                            {
                                c_side.push_back(x);
                            }
                        }
                    }
                }
            }
        }

        return inter;
    }
}

void U10084::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
