#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u118.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <functional>
#include <list>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U118 instance;
    instance();
}

namespace {

    enum class orientation_t : char
    {
        N = 'N', S = 'S', W = 'W', E = 'E'
    };

    bool operator == (orientation_t lhs, orientation_t rhs)
    {
        return static_cast<char>(lhs) == static_cast<char>(rhs);
    }

    class point_t : std::tuple<int8_t, int8_t> {
    public:
        point_t (int8_t x = 0, int8_t y = 0) : std::tuple<int8_t, int8_t>(x, y) {}

        point_t& operator = (const point_t& rhs)
        {
            x() = rhs.x();
            y() = rhs.y();
            return *this;
        }

        friend bool operator == (const point_t& lhs, const point_t& rhs)
        {
            return lhs.x() == rhs.x() && lhs.y() == rhs.y();
        }

        int8_t& x()
        {
            return std::get<0>(*this);
        }
        int8_t& y()
        {
            return std::get<1>(*this);
        }

        int8_t x() const
        {
            return std::get<0>(*this);
        }
        int8_t y() const
        {
            return std::get<1>(*this);
        }

        point_t& move (orientation_t head)
        {
            switch (head)
            {
            case orientation_t::N:
                y()++;
                break;

            case orientation_t::S:
                y()--;
                break;

            case orientation_t::E:
                x()++;
                break;

            case orientation_t::W:
                x()--;
                break;

            default:
                break;
            }

            return *this;
        }

        friend std::istream& operator >> (std::istream& in, point_t& point)
        {
            uint16_t x(0), y(0);
            in >> x >> y;
            point.x() = x;
            point.y() = y;
            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const point_t& point)
        {
            out << static_cast<int16_t>(point.x()) << ' ' << static_cast<int16_t>(point.y());
            return out;
        }
    };

    enum class instruction_t : char
    {
        R = 'R', L = 'L', F = 'F'
    };

    class robot_t : std::tuple<point_t, bool, orientation_t> {
    public:
        robot_t(point_t& pos, orientation_t head) : std::tuple<point_t, bool, orientation_t>(pos, false, head) {}
        robot_t() : std::tuple<point_t, bool, orientation_t>(point_t(0, 0), false, orientation_t::N) {}

        point_t& position()
        {
            return std::get<0>(*this);
        }
        bool& alive()
        {
            return std::get<1>(*this);
        }
        orientation_t& heading()
        {
            return std::get<2>(*this);
        }

        point_t position() const
        {
            return std::get<0>(*this);
        }
        bool alive() const
        {
            return std::get<1>(*this);
        }
        orientation_t heading() const
        {
            return std::get<2>(*this);
        }

        robot_t& move(instruction_t instruction, std::function<bool(point_t&)> within, std::function<bool(point_t&)> dropoff)
        {
#if 0
            std::cerr << *this <<  ": " << static_cast<char>(instruction);
#endif

            switch (instruction)
            {
            case instruction_t::R:
            case instruction_t::L:
                rotate(instruction);
                break;

            case instruction_t::F:
            {
                point_t next (position());
                next.move(heading());

                if (not within(next))
                {
                    alive() = not dropoff(position());
                }
                else
                {
                    position() = next;
                }

                break;
            }

            default:
                break;
            }

#if 0
            std::cerr << " = " << *this << std::endl;
#endif
            return *this;
        }

        void reset()
        {
            position() = point_t(0, 0);
            alive() = false;
            heading() = orientation_t::N;
        }

        friend std::istream& operator >> (std::istream& in, robot_t& robot)
        {
            char head(0);
            in >> robot.position() >> head;
            robot.heading() = static_cast<orientation_t>(head);
            robot.alive() = true;
            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const robot_t& robot)
        {
            out << robot.position() << ' ' << static_cast<char>(robot.heading());

            if (not robot.alive())
            {
                out << " LOST";
            }

            return out;
        }

    private:
        template<typename Iterator>
        orientation_t doRotate(Iterator begin, Iterator end, orientation_t heading)
        {
            Iterator current = std::find(begin, end, heading);
            ++current;

            if (current == end)
            {
                current = begin;
            }

            return *current;
        }

        void rotate(instruction_t instruction)
        {
            static std::vector<orientation_t> headings ({orientation_t::N, orientation_t::E, orientation_t::S, orientation_t::W});

            if (instruction == instruction_t::R)
            {
                heading() = doRotate(headings.begin(), headings.end(), heading());
            }
            else
            {
                heading() = doRotate(headings.rbegin(), headings.rend(), heading());
            }
        }
    };

    class solution {
    public:
        solution() : X_(), Y_(), robot_(), instructions_(), off_points_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }

        solution& operator()();

    private:
        uint16_t X_, Y_;
        robot_t robot_;
        std::string instructions_;
        std::list<point_t> off_points_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        if (sol.X_ == 0 && sol.Y_ == 0)
        {
            in >> sol.X_ >> sol.Y_;
        }

        sol.robot_.reset();
        sol.instructions_.clear();
        in >> sol.robot_ >> sol.instructions_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.robot_;
        return out;
    }

    solution& solution::operator()()
    {
        std::stringstream in (instructions_);
        char ch;

        while (in >> ch && robot_.alive())
        {
            robot_.move(static_cast<instruction_t>(ch), [&](point_t& next) -> bool
            {
                return next.x() >= 0 && next.x() <= X_ && next.y() >= 0 && next.y() <= Y_;
            }, [&](point_t& point) -> bool
            {
                return std::find(off_points_.begin(), off_points_.end(), point) == off_points_.end();
            });
        }

        if (not robot_.alive())
        {
            off_points_.push_back(robot_.position());
        }

        return *this;
    }

}

void U118::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
