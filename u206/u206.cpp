#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u206.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U206 instance;
    instance();
}

namespace {

    class customer {
    public:
        std::string name_;
        int32_t x_, y_;
        long double angle_;
        long double distance_;

        customer() : name_(), x_(), y_(), angle_(), distance_() {}

        friend std::istream& operator >> (std::istream& in, customer& cust)
        {
        	static long double _0 = std::atan2(1, 0);
            std::string line;
            std::getline(in, cust.name_);
            std::getline(in, line);
            std::stringstream strstream(line);
            strstream >> cust.x_ >> cust.y_;
            cust.distance_ = std::sqrt(cust.x_ * cust.x_ + cust.y_ * cust.y_); // @suppress("Invalid arguments")
            cust.angle_ = std::atan2(cust.x_, cust.y_) + _0; // @suppress("Invalid arguments")
            return in;
        }
    };

    class solution {
        std::vector<std::shared_ptr<customer>> customers_;
        std::vector<std::list<std::shared_ptr<customer>>> routes_;
        std::string title_;
    public:
        solution();

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return not customers_.empty();
        }
        solution& operator()();

    private:
    };

    solution::solution() : customers_(), routes_(), title_() {}

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.customers_.clear();
        sol.routes_.clear();

        if (std::getline(in, sol.title_)) {
            std::string line;
            std::getline(in, line);
            std::stringstream temp(line);
            size_t n_cust, n_routes;
            temp >> n_routes >> n_cust;
            sol.routes_.resize(n_routes);
            sol.customers_.reserve(n_cust);

            while (n_cust--) {
                auto cust = std::make_shared<customer>();
                in >> *cust;
                sol.customers_.push_back(cust);
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.title_ << std::endl << "Number of Customers: " << sol.customers_.size() << "Number of Routes: " << sol.routes_.size() << std::endl;
        auto idx(0);
        auto total_lengh(0);
        std::for_each(sol.routes_.begin(), sol.routes_.end(), [&](auto & route) {
            out << std::endl << "Route ==> " << ++idx << std::endl;
            auto route_lenght(0);
            auto last_x(0), last_y(0);
            std::for_each(route.begin(), route.end(), [&](auto cust) {
                out << "Customer: " << cust->name_ << std::endl;;
                route_lenght += std::abs(cust->x_) + std::abs(cust->y_);
                last_x = cust->x_;
                last_y = cust->y_;
            });
            route_lenght += std::abs(last_x) + std::abs(last_y);
            out << "Route length ==> " << route_lenght << std::endl;
            total_lengh += route_lenght;
        });
        out << std::endl << "Total Route Length ==> " << total_lengh << std::endl << "***********************************" << std::endl;
        return out;
    }

    solution& solution::operator()()
    {
        std::sort(customers_.begin(), customers_.end(), [](auto c1, auto c2) {
            if (c1->angle_ > c2->angle_) {
                return true;
            } else if (c1->angle_ == c2->angle_) {
                return c1->distance_ < c2->distance_;
            } else {
                return false;
            }
        });
        auto cust_per_route = customers_.size() / routes_.size();
        std::vector<size_t> route_sizes;
        route_sizes.reserve(routes_.size());
        std::generate_n(std::back_inserter(route_sizes), routes_.size(), [ = ]() {
            return cust_per_route;
        });

        auto r_sizes_it = route_sizes.begin();

        while (std::accumulate(route_sizes.begin(), route_sizes.end(), 0ul) != customers_.size()) {
            (*r_sizes_it)++;
            ++r_sizes_it;
        }

        auto cust = customers_.begin();
        r_sizes_it = route_sizes.begin();

        for (auto routeIt = routes_.begin(); routeIt != routes_.end(); ++routeIt, ++r_sizes_it) {
            for (size_t idx = 0; idx < *r_sizes_it && cust != customers_.end(); idx++) {
                routeIt->push_back(*cust);
                ++cust;
            }
        }

        return *this;
    }
}

void U206::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
