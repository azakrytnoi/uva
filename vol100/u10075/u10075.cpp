#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10075.h"

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

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10075 instance;
    instance();
}

namespace {

    template<int64_t R>
    struct geo_point_t
    {
        double_t x_, y_, z_;

        geo_point_t(double_t lat, double_t lon)
            : x_(std::cos(lat) * std::sin(lon)),
              y_(std::cos(lat) * std::cos(lon)),
              z_(std::sin(lat))
        {
            *this *= R;
        }

        geo_point_t ()
            : x_(), y_(), z_() {}

        geo_point_t& operator *= (double_t mul)
        {
            x_ *= mul, y_ *= mul, z_ *= mul;
            return *this;
        }

        static double_t distance(geo_point_t& p1, geo_point_t& p2)
        {
            auto AB = std::sqrt(std::pow(p1.x_ - p2.x_, 2) + std::pow(p1.y_ - p2.y_, 2) + std::pow(p1.z_ - p2.z_, 2));
            auto theta = std::acos((2.0 * R * R - AB * AB) / (2.0 * R * R));
            return R * theta;
        }
    };

    class solution_t {
    public:
        solution_t() : case_no_(), N_(), M_(), Q_(), cities_(), routes_(), queries_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return N_ + M_ + Q_ != 0;
        }
        solution_t& operator()();

    private:
        constexpr static int64_t R_ = 6378;

        size_t case_no_;
        size_t N_, M_, Q_;
        std::map<std::string, std::pair<size_t, geo_point_t<R_>>> cities_;
        std::vector<std::pair<std::string, std::string>> routes_;
        std::vector<std::pair<std::string, std::string>> queries_;
        std::vector<int64_t> result_;
    };
    template<typename _Container>
    class map_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
    private:
        _Container* container_;

    public:
        typedef _Container          container_type;

        explicit
        map_insert_iterator(_Container& __x)
            : container_(std::__addressof(__x)) { }

        map_insert_iterator& operator=(const typename _Container::value_type& __val)
        {
            container_->insert(std::make_pair(__val.first, __val.second));
            return *this;
        }

        map_insert_iterator& operator=(typename _Container::value_type&& __val)
        {
            container_->insert(std::make_pair(std::move(__val.first), std::move(__val.second)));
            return *this;
        }

        map_insert_iterator& operator*()
        {
            return *this;
        }

        map_insert_iterator& operator++()
        {
            return *this;
        }

        map_insert_iterator operator++(int)
        {
            return *this;
        }
    };

    template<typename _Container>
    inline map_insert_iterator<_Container>
    map_inserter(_Container& __x)
    {
        return map_insert_iterator<_Container>(__x);
    }


    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.N_ >> sol.M_ >> sol.Q_;

        if (sol)
        {
            sol.cities_.clear();
            std::generate_n(map_inserter(sol.cities_), sol.N_, [&]()
            {
                std::string name;
                double_t lat(0), lon(0);
                in >> name >> lat >> lon;
                lat *= M_PI / 180.0, lon *= M_PI / 180.0;
                geo_point_t<sol.R_> pos (lat, lon);
                return std::make_pair(name, std::make_pair(sol.cities_.size(), pos));
            });

            sol.routes_.clear();
            sol.routes_.reserve(sol.M_);
            std::generate_n(std::back_inserter(sol.routes_), sol.M_, [&]()
            {
                std::string from, to;
                in >> from >> to;
                return std::make_pair(from, to);
            });

            sol.queries_.clear();
            sol.queries_.reserve(sol.Q_);
            std::generate_n(std::back_inserter(sol.queries_), sol.Q_, [&]()
            {
                std::string from, to;
                in >> from >> to;
                return std::make_pair(from, to);
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Case #" << sol.case_no_ << std::endl;

        for (auto res : sol.result_)
        {
            if (res == std::numeric_limits<int32_t>::max())
            {
                out << "no route exists" << std::endl;
            }
            else
            {
                out << res << " km" << std::endl;
            }
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        ++case_no_;
        std::vector<std::vector<int64_t>> dp(N_, std::vector<int64_t>(N_, std::numeric_limits<int32_t>::max()));

        for (auto i = 0u; i < N_; ++i)
        {
            dp[i][i] = 0;
        }

        for (auto& r : routes_)
        {
            auto& v1(cities_[r.first]);
            auto& v2(cities_[r.second]);

            if (v1.first != v2.first)
            {
                dp[v1.first][v2.first] = (int64_t)(geo_point_t<R_>::distance(v1.second, v2.second) + 0.5);
            }
        }

        for (auto k = 0u; k < N_; ++k)
        {
            for (auto i = 0u; i < N_; ++i)
            {
                for (auto j = 0u; j < N_; ++j)
                {
                    dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k][j]);
                }
            }
        }

        result_.clear();
        result_.reserve(queries_.size());
        std::transform(queries_.begin(), queries_.end(), std::back_inserter(result_), [&](auto & q)
        {
            auto& v1(cities_[q.first]);
            auto& v2(cities_[q.second]);
            return dp[v1.first][v2.first];
        });
        return *this;
    }

}

void U10075::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
