#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10078.h"

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

#include "point.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10078 instance;
    instance();
}

namespace geom {

    std::istream& operator >> (std::istream& in, geom::point_t& point)
    {
        in >> point.x_ >> point.y_;
        return in;
    }

} // namespace geom

namespace {

    class solution_t {
    public:
        solution_t() : points_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not points_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<geom::point_t> points_;
        bool result_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.points_.clear();
        size_t n(0);

        if (in >> n && n > 0)
        {
            sol.points_.reserve(n);
            std::istream_iterator<geom::point_t> pin(in);
            std::copy_n(pin, n, std::back_inserter(sol.points_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << (sol.result_ ? "Yes" : "No");
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<geom::point_t> deltas;
        deltas.reserve(points_.size());

        for (auto i = 0u; i < points_.size(); ++i)
        {
            auto j ((i + 1) % points_.size());
            deltas.push_back(points_[j] - points_[i]);
        }

        std::vector<double_t> t;
        t.reserve(deltas.size());

        for (auto i = 0u; i < deltas.size(); ++i)
        {
            auto j ((i + 1) % deltas.size());
            t.push_back(deltas[i].x_ * deltas[j].y_ - deltas[i].y_ * deltas[j].x_);
        }

        auto check = [&]() -> bool
        {
            if (t[0] < 0)
            {
                for (auto i = 1u; i < t.size(); ++i)
                    if (t[i] > 0)
                    {
                        return true;
                    }
            }
            else
            {
                for (auto i = 1u; i < t.size(); ++i)
                    if (t[i] < 0)
                    {
                        return true;
                    }
            }

            return false;
        };
        result_ = check();
        return *this;
    }

}

void U10078::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
