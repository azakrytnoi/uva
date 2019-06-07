#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u184.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <list>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U184 instance;
    instance();
}

namespace {

    typedef std::pair<uint16_t, uint16_t> point2d;
    typedef std::pair<point2d, point2d> line2d;

    class solution {
    public:
        solution() :
            points_(), lines_()
        {
            points_.reserve(300);
        }

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            sol.points_.clear();

            while (in)
            {
                point2d point;
                in >> point.first >> point.second;

                if (point.first == 0 && point.second == 0)
                {
                    break;
                }

                sol.points_.push_back(point);
            }

            std::sort(sol.points_.begin(), sol.points_.end(), [](auto p1, auto p2)
            {
                if (p1.first == p2.first)
                {
                    return p1.second < p2.second;
                }

                return p1.first < p2.first;
            });
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol)
        {
            std::for_each(sol.lines_.begin(), sol.lines_.end(), [&](const std::list<point2d>& line)
            {
                std::for_each(line.begin(), line.end(), [&](auto point)
                {
                    out << "(" << std::setw(4) << point.first << "," << std::setw(4) << point.second << ")";
                });
                out << std::endl;
            });
            return out;
        }

        bool solve();
        bool empty() const
        {
            return points_.empty();
        }

    private:
        std::vector<point2d> points_;
        std::list<std::list<point2d>>lines_;

        static
        bool is_colinear(point2d& a, point2d& b, point2d& c)
        {
            return a.first * (b.second - c.second) + b.first * (c.second - a.second) + c.first * (a.second - b.second) == 0;
        }

        static
        bool is_extending(point2d& a, point2d& b, std::list<line2d>& proceesed)
        {
            for (auto pp = proceesed.begin(); pp != proceesed.end(); ++pp)
            {
                bool r1 (is_colinear(a, b, pp->first));
                bool r2 (is_colinear(a, b, pp->second));

                if ( r1 && r2 )
                {
                    return true;
                }
            }

            return false;
        }
    };

    bool solution::solve()
    {
        lines_.clear();
        std::list<line2d> processed;

        for (auto pa = points_.begin(); pa != points_.end() - 2; ++pa)
        {
            for (auto pb = pa + 1; pb != points_.end() - 1; ++pb)
            {
                if (!is_extending(*pa, *pb, processed))
                {
                    std::list<point2d> line;

                    for (auto pc = pb + 1; pc != points_.end(); ++pc)
                    {
                        if (is_colinear(*pa, *pb, *pc))
                        {
                            if (line.empty())
                            {
                                line.push_back(*pa);
                                line.push_back(*pb);
                                processed.push_back(std::make_pair(*pa, *pb));
                            }

                            line.push_back(*pc);
                        }
                    }

                    if (!line.empty())
                    {
                        lines_.push_back(line);
                    }
                }
            }
        }

        return !lines_.empty();
    }
}

void U184::operator()() const
{
    solution sol;

    while (std::cin >> sol && !sol.empty())
    {
        if (sol.solve())
        {
            std::cout << "The following lines were found:" << std::endl;
            std::cout << sol;
        }
        else
        {
            std::cout << "No lines were found" << std::endl;
        }
    }
}
