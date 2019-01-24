#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u109.h"

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
    U109 instance;
    instance();
}

namespace {

    struct point_t : std::pair<int16_t, int16_t> {
        point_t(int16_t x = 0, int16_t y = 0) : std::pair<int16_t, int16_t>(x, y) {}

        int16_t& x()
        {
            return first;
        }
        int16_t x() const
        {
            return first;
        }
        int16_t& y()
        {
            return second;
        }
        int16_t y() const
        {
            return second;
        }

        static int32_t cross (const point_t& a, const point_t& b, const point_t& c)
        {
            auto x1 = a.x() - c.x();
            auto x2 = b.x() - c.x();
            auto y1 = a.y() - c.y();
            auto y2 = b.y() - c.y();
            return x1 * y2 - x2 * y1;
        }

        friend std::istream& operator >> (std::istream& in, point_t& point)
        {
            in >> point.x() >> point.y();
            return in;
        }
    };

    class kingdom_t {
    public:
    	kingdom_t(): points_(), hull_(), alive_(true) {}

    	std::vector<point_t> points_;
    	std::vector<point_t> hull_;
    	bool alive_;
    };

    class solution_t {
    public:
        solution_t() : kingdoms_(), total_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        solution_t& operator()(std::istream& in);

    private:
        std::vector<kingdom_t> kingdoms_;
        double_t total_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.kingdoms_.reserve(20);
        int32_t m (0);

        while (in >> m && m != -1) {
            sol.kingdoms_.push_back(kingdom_t());
            auto kingdom = --sol.kingdoms_.end();
            kingdom->points_.reserve(m);
            std::generate_n(std::back_inserter(kingdom->points_), m, [&]() {
                point_t pp;
                in >> pp;
                return pp;
            });
            {
                auto median = kingdom->points_.begin();

                for (auto point = kingdom->points_.begin() + 1; point != kingdom->points_.end(); ++point) {
                    if (point->x() < median->x() || (point->x() == median->x() && point->y() < median->y())) {
                        median = point;
                    }
                }

                std::swap(kingdom->points_[0], *median);
                std::sort(kingdom->points_.begin() + 1, kingdom->points_.end(), [&](const point_t& lhs, const point_t& rhs) {
                    return point_t::cross(kingdom->points_[0], lhs, rhs) < 0;
                });
                kingdom->points_.push_back(kingdom->points_[0]);
            }
            kingdom->hull_.push_back(kingdom->points_[0]);
            kingdom->hull_.push_back(kingdom->points_[1]);

            for (auto point = kingdom->points_.begin() + 2; point != kingdom->points_.end(); ++point) {
                while (kingdom->hull_.size() >= 2 && point_t::cross(kingdom->hull_[kingdom->hull_.size() - 2], kingdom->hull_[kingdom->hull_.size() - 1], *point) > 0) {
                	kingdom->hull_.erase(kingdom->hull_.end());
                }

                kingdom->hull_.push_back(*point);
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << std::fixed << std::setprecision(2) << sol.total_;
        return out;
    }

    solution_t& solution_t::operator()(std::istream& in)
    {
        point_t fire;

        while (in >> fire) {
            for (auto kingdom = kingdoms_.begin(); kingdom != kingdoms_.end(); ++kingdom) {
                if (kingdom->alive_) {
                    bool mark = true;

                    for (auto hull = kingdom->hull_.begin() + 1; hull != kingdom->hull_.end(); ++hull) {
                        if (point_t::cross(*(hull - 1), *hull, fire) > 0) {
                            mark = false;
                            break;
                        }
                    }

                    if (mark) {
                        kingdom->alive_ = false;
                        double_t area(0);

                        for (auto hull = kingdom->hull_.begin() + 1; hull != kingdom->hull_.end(); ++hull) {
                            area -= (hull - 1)->x() * hull->y() - hull->x() * (hull - 1)->y();
                        }

                        area /= 2.0;
                        total_ += area;
                    }
                }
            }
        }

        return *this;
    }

}

void U109::operator()() const
{
    solution_t sol;

    std::cin >> sol;
    std::cout << sol(std::cin) << std::endl;
}
