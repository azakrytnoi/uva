#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u203.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>
#include <memory>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U203 instance;
    instance();
}

namespace {
    class point_t {
    public:
        point_t(double_t x = 0, double_t y = 0) : x_(x), y_(y) {}
        point_t(const point_t& p) : x_(p.x_), y_(p.y_) {}

        double_t x_, y_;

        friend point_t operator +(const point_t& p1, const point_t& p2)
        {
            return point_t(p1.x_ + p2.x_, p1.y_ + p2.y_);
        }

        static double_t distance(const point_t& p1, const point_t& p2)
        {
            return std::sqrt((p1.x_ - p2.x_) * (p1.x_ - p2.x_) //
                             + (p1.y_ - p2.y_) * (p1.y_ - p2.y_));
        }

        static double_t bearing(const point_t& a, const point_t& b, const point_t& c)
        {
            point_t ab = {b.x_ - a.x_, b.y_ - a.y_};
            point_t cb = {b.x_ - c.x_, b.y_ - c.y_};
            double_t dot = (ab.x_ * cb.x_ + ab.y_ * cb.y_);
            double_t cross = (ab.x_ * cb.y_ - ab.y_ * cb.x_);
            double_t alpha = std::atan2(cross, dot);
            return alpha * 180.0 / M_PI + 180.0;
        };

        static std::pair<bool, point_t> crossing(const point_t& p1, const point_t& p2, const point_t& p3, const point_t& p4)
        {
            auto det1 = det(p1, p2);
            auto det2 = det(p3, p4);
            auto x1mx2 = p1.x_ - p2.x_;
            auto x3mx4 = p3.x_ - p4.x_;
            auto y1my2 = p1.y_ - p2.y_;
            auto y3my4 = p3.y_ - p4.y_;
            auto xnom = det({det1, x1mx2}, {det2, x3mx4});
            auto ynom = det({det1, y1my2}, {det2, y3my4});
            auto denom = det({x1mx2, y1my2}, {x3mx4, y3my4});

            if (denom == 0.0)
            {
                return std::make_pair(false, point_t());
            }

            return std::make_pair(true, point_t(xnom / denom, ynom / denom));
        };

        static bool between(const point_t& point, const point_t& start, const point_t& end)
        {
            double_t x[] = {start.x_, end.x_};
            std::sort(x, x + 2);
            return point.x_ >= x[0] && point.x_ <= x[1];
        };

    private:
        static double_t det(const point_t& p1, const point_t& p2)
        {
            return p1.x_ * p2.y_ - p1.y_ * p2.x_;
        };
    };

    class ship_t {
    public:
        ship_t(): name_(), point_(), next_(), course_(), speed_(), bearing_(), distance_() {}
        ship_t(ship_t&& rhs) : name_(std::move(rhs.name_)), point_(rhs.point_), next_(rhs.next_), course_(rhs.course_),
            speed_(rhs.speed_), bearing_(rhs.bearing_), distance_(rhs.distance_) {}

        void reset()
        {
            name_.clear();
            point_.x_ = point_.y_ = next_.x_ = next_.y_ = course_ = speed_ = bearing_ = 0.0;
        }

        friend std::istream& operator >>(std::istream& in, ship_t& ship)
        {
            std::getline(in, ship.name_);
            std::string temp;
            std::getline(in, temp);
            std::stringstream sin(temp);
            sin >> ship.point_.x_ >> ship.point_.y_ >> ship.course_ >> ship.speed_;
            ship.course_ = 90 - ship.course_;

            if (std::abs(ship.course_) > 180)
            {
                ship.course_ += ship.course_ > 0 ? -360 : 360;
            }

            ship.course_ *= M_PI / 180.0;
            double _3m_distance = ship.speed_ * 3 / 60;
            ship.next_ = {_3m_distance* std::cos(ship.course_), _3m_distance* std::sin(ship.course_)};
            return in;
        }

        std::string name_;
        point_t point_, next_;
        double_t course_, speed_;
        double_t bearing_, distance_;
    };

    class solution_t {
    public:
        solution_t() : set_name_(), own_(), others_(), collisions_() {}

        operator bool()
        {
            return not set_name_.empty();
        }

        solution_t& operator()()
        {
            for (auto& ship : others_)
            {
                ship.distance_ = point_t::distance(own_.point_, ship.point_);
                ship.bearing_ = point_t::bearing(ship.point_, own_.point_, ship.next_);

                if (ship.distance_ <= 10.0)
                {
//                    auto next_bearing = point_t::bearing(ship.point_ + ship.next_, own_.point_ + own_.next_, ship.next_ + ship.next_);
                    auto next_distance = point_t::distance(own_.point_ + own_.next_, ship.point_ + ship.next_);

                    if (ship.distance_ >= next_distance)
                    {
                        collisions_.push_back(&ship);
                    }
                }
            }

            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            sol.set_name_.clear();
            sol.others_.clear();
            sol.collisions_.clear();
            sol.own_.reset();

            if (std::getline(in, sol.set_name_))
            {
                size_t n;
                in >> n;
                std::string tmp;
                std::getline(in, tmp);
                in >> sol.own_;
                sol.others_.reserve(n);
                std::generate_n(std::back_inserter(sol.others_), n, [&]()
                {
                    ship_t ship;
                    in >> ship;
                    return ship;
                });
                sol.collisions_.reserve(n);
            }

            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            auto visible_lights = [](double_t angle) -> std::string
            {
                std::string result("Masthead");

                if (angle >= 357.5 || angle <= 115.0)
                {
                    result += " Green";
                }

                if (angle >= 110.0 && angle <= 250.0)
                {
                    result += " Stern";
                }

                if (angle >= 245.0 || angle <= 2.5)
                {
                    result += " Red";
                }

                return result;
            };
            out << "Scenario: " << sol.set_name_ << std::endl << std::endl
                << R"(Boat ID       Bearing      Distance     Lights (left to right)
---------------------------------------------------------------)" << std::endl;

            for (auto& ship : sol.others_)
            {
                out << std::setw(12) << std::left << ship.name_ << std::setw(2) << ' ' //
                    << std::fixed << std::setw(7) << std::right << std::setprecision(2) << ship.bearing_ //
                    << std::setw(12) << ship.distance_ << std::setw(7) << ' ' //
                    << (ship.distance_ > 10.0 ? "Lights not visible" : visible_lights(ship.bearing_)) << std::endl;
            }

            if (not sol.collisions_.empty())
            {
                for (auto& ship : sol.collisions_)
                {
                    auto distance = point_t::distance(sol.own_.point_ + sol.own_.next_, ship->point_ + ship->next_);
                    out << "** Collision warning --> " << ship->name_ << ":  Distance = " << std::fixed << std::setprecision(2) << distance << std::endl;
                }
            }

            out << "***************************************************************" << std::endl;
            return out;
        }

    private:
        std::string set_name_;
        ship_t own_;
        std::vector<ship_t> others_;
        std::vector<ship_t*> collisions_;
    };

}

void U203::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
