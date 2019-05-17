#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10080.h"

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
#include <functional>

#include "point.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10080 instance;
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
        solution_t() : gophers_(), holes_(), max_time_(), velocity_(), eaten_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not gophers_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<geom::point_t> gophers_;
        std::vector<geom::point_t> holes_;
        int16_t max_time_, velocity_;
        size_t eaten_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.gophers_.clear();
        sol.holes_.clear();
        size_t n(0), m(0);

        if (in >> n >> m >> sol.max_time_ >> sol.velocity_)
        {
            std::istream_iterator<geom::point_t> pin(in);
            sol.gophers_.reserve(n);
            std::copy_n(pin, n, std::back_inserter(sol.gophers_));
            sol.holes_.reserve(m);
            std::copy_n(pin, m, std::back_inserter(sol.holes_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.eaten_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto matrix_size(gophers_.size() + holes_.size());
        std::vector<int16_t> match(matrix_size, -1);
        std::vector<std::vector<size_t>> dp(matrix_size);
        std::function<size_t(size_t, std::vector<bool>&)> augmenting_path = [&](size_t idx, std::vector<bool>& visited)
        {
            if (visited[idx])
            {
                return 0;
            }

            visited[idx] = true;

            for (auto j = 0u; j < dp[idx].size(); ++j)
            {
                auto r = dp[idx][j];

                if (match[r] == -1 || augmenting_path(match[r], visited))
                {
                    match[r] = idx;
                    return 1;
                }
            }

            return 0;
        };

        for (auto g = 0u; g < gophers_.size(); ++g)
        {
            for (auto h = 0u; h < holes_.size(); ++h)
            {
                if (max_time_ > (geom::point_t::distance(gophers_[g], holes_[h]) / (double) velocity_))
                {
                    dp[g].push_back(gophers_.size() + h);
                }
            }
        }

        size_t saved(0);

        for (auto idx = 0u; idx < gophers_.size(); ++idx)
        {
            std::vector<bool> visited(gophers_.size(), false);
            saved += augmenting_path(idx, visited);
        }

        eaten_ = gophers_.size() - saved;
        return *this;
    }

}

void U10080::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
