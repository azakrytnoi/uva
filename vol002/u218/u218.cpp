#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u218.h"

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
    U218 instance;
    instance();
}

namespace {
    class point : std::pair<double_t, double_t> {
    public:
        point(double_t x = 0, double_t y = 0) : std::pair<double_t, double_t>(x, y) {}

        double_t x() const
        {
            return first;
        }
        double_t y() const
        {
            return second;
        }

        bool operator < (const point& rhs) const
        {
            if (first != rhs.first)
            {
                return first < rhs.first;
            }

            return second < rhs.second;
        }

        friend std::ostream& operator << (std::ostream& out, const point& p)
        {
            out << std::fixed << std::setprecision(1) << "(" << p.x() << "," << p.y() << ")";
            return out;
        }

        friend std::istream& operator >> (std::istream& in, point& p)
        {
            in >> p.first >> p.second;
            return in;
        }
    };

    class solution {
        std::vector<point> points_;
        std::vector<point> perimeter_;
        static size_t case_no_;
    public:
        solution() : points_(), perimeter_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator << (std::ostream& out, const solution& sol);

        operator bool ()
        {
            return not points_.empty();
        }
        solution& operator()();

    private:
        double area(point& a, point& b, point& c)
        {
            return a.x() * b.y() + b.x() * c.y() + c.x() * a.y() - a.y() * b.x() - b.y() * c.x() - c.y() * a.x();
        }
    };

    size_t solution::case_no_ = 0;

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.points_.clear();
        sol.perimeter_.clear();
        size_t n;
        in >> n;

        if (n != 0)
        {
            sol.case_no_++;
            sol.points_.reserve(n);
            std::generate_n(std::back_inserter(sol.points_), n, [&]()
            {
                point p;
                in >> p;
                return p;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << "Region #" << sol.case_no_ << ":" << std::endl;

        for (auto pItr = sol.perimeter_.rbegin(); pItr != sol.perimeter_.rend(); ++ pItr)
        {
            if (pItr != sol.perimeter_.rbegin())
            {
                out << "-";
            }

            out << *pItr;
        }

        out << std::endl;
        double perimeter(0);

        for (size_t idx = 0 ; idx < sol.perimeter_.size() - 1; idx++)
        {
            perimeter += std::sqrt( //
                             std::pow(sol.perimeter_[idx].x() - sol.perimeter_[idx + 1].x(), 2) + //
                             std::pow(sol.perimeter_[idx].y() - sol.perimeter_[idx + 1].y(), 2));
        }

        out << "Perimeter length = " << std::fixed << std::setprecision(2) << perimeter << std::endl;
        return out;
    }

    solution& solution::operator()()
    {
        std::sort(points_.begin(), points_.end());
        perimeter_.resize(points_.size() * 2);
        size_t per_idx(0);

        for (size_t idx = 0; idx < points_.size(); idx++)
        {
            while (per_idx >= 2 && area(perimeter_[per_idx - 2], perimeter_[per_idx - 1], points_[idx]) <= 0)
            {
                per_idx--;
            }

            perimeter_[per_idx++] = points_[idx];
        }

        //  deepcode ignore CppLoopCheckAndUpdateMismatch: false positive, comparing unsigned with 0 doesn't work here
        for (size_t idx = points_.size() - 2, top_idx = per_idx + 1; idx < points_.size() ; idx--)
        {
            while (per_idx >= top_idx && area(perimeter_[per_idx - 2], perimeter_[per_idx - 1], points_[idx]) <= 0)
            {
                per_idx--;
            }

            perimeter_[per_idx++] = points_[idx];
        }

        perimeter_.resize(per_idx);

        if (perimeter_.size() == 1)
        {
            perimeter_.push_back(perimeter_[0]);
        }

        return *this;
    }
}

void U218::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
