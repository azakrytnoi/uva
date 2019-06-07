#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10043.h"

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

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10043 instance;
    instance();
}

namespace {

    struct point_t
    {
        int32_t x_, y_;

        point_t(int32_t x = 0, int32_t y = 0) : x_(x), y_(y) {}
    };

    class solution_t {
    public:
        solution_t(): N_(std::numeric_limits<size_t>::max()), l_(), w_(), tree_(), area_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return true;
        }
        solution_t& operator()();

    private:
        size_t N_;
        int32_t l_, w_;
        std::vector<point_t> tree_;
        int32_t area_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        sol.tree_.clear();

        if (in >> sol.l_ >> sol.w_)
        {
            sol.tree_.reserve(3000);
            sol.tree_.push_back({0, 0}), sol.tree_.push_back({sol.l_, sol.w_}), sol.tree_.push_back({sol.l_, 0}), sol.tree_.push_back({0, sol.w_});
            int32_t op;
            int32_t x(0), y(0), dx(0), dy(0);

            while (in >> op && op > 0)
            {
                in >> x >> y;

                if (op != 1)
                {
                    in >> dx >> dy;
                }

                std::generate_n(std::back_inserter(sol.tree_), op, [&]()
                {
                    point_t ret(x, y);
                    x += dx, y += dy;
                    return ret;
                });
            }

            sol.tree_.shrink_to_fit();
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.area_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        area_ = 0;

        std::sort(tree_.begin(), tree_.end(), [](const point_t& lhs, const point_t& rhs)
        {
            if (lhs.x_ == rhs.x_)
            {
                return lhs.y_ < rhs.y_;
            }

            return lhs.x_ < rhs.x_;
        });

        for (auto i = tree_.begin(); i != tree_.end(); ++i)
        {
            auto mny(0), mxy(w_);

            for (auto j = i + 1; j != tree_.end(); ++j)
            {
                area_ = std::max(area_, (j->x_ - i->x_) * (mxy - mny));

                if (j->x_ == i->x_)
                {
                    continue;
                }

                if (j->y_ > i->y_)
                {
                    mxy = std::min(mxy, j->y_);
                }
                else
                {
                    mny = std::max(mny, j->y_);
                }
            }
        }

        std::sort(tree_.begin(), tree_.end(), [](const point_t& lhs, const point_t& rhs)
        {
            if (lhs.y_ == rhs.y_)
            {
                return lhs.x_ < rhs.x_;
            }

            return lhs.y_ < rhs.y_;
        });

        for (auto i = tree_.begin(); i != tree_.end(); ++i)
        {
            auto mnx(0), mxx(l_);

            for (auto j = i + 1; j != tree_.end(); ++j)
            {
                area_ = std::max(area_, (j->y_ - i->y_) * (mxx - mnx));

                if (j->y_ == i->y_)
                {
                    continue;
                }

                if (j->x_ > i->x_)
                {
                    mxx = std::min(mxx, j->x_);
                }
                else
                {
                    mnx = std::max(mnx, j->x_);
                }
            }
        }

        return *this;
    }

}

void U10043::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
