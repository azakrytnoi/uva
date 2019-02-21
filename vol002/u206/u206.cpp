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

    class location_t {
    public:
        location_t (int32_t x = 0, int32_t y = 0, const std::string& name = "") : x_(x), y_(y), name_(name) {}

        double_t phi() const
        {
            double_t _phi = std::atan2(y_, x_);

            if (_phi < 0) {
                _phi += 3 * M_PI;
            }

            return _phi;
        }

        double_t r2() const
        {
            return x_ * x_ + y_ * y_;
        }

        int32_t x() const
        {
            return x_;
        }

        int32_t y() const
        {
            return y_;
        }

        std::string name() const
        {
            return name_;
        }

        bool operator <(const location_t& rhs) const
        {
            auto a = phi();
            auto b = rhs.phi();

            if (std::abs(a - b) >= 1e-7) {
                return a < b;
            }

            return r2() < rhs.r2();
        }

        friend std::istream& operator >>(std::istream& in, location_t& loc)
        {
            std::getline(in, loc.name_);
            std::getline(in, loc.name_);
            in >> loc.x_ >> loc.y_;
            return in;
        }

    private:
        int32_t x_, y_;
        std::string name_;
    };

    class route_t {
    public:
        route_t() : cost_(), customers_() {}

        size_t cost() const
        {
            return cost_;
        }
        void populate (std::vector<location_t>::iterator& current, size_t capacity);

        friend std::ostream& operator<<(std::ostream& out, const route_t& route);
    private:
        size_t cost_;
        std::vector<location_t*> customers_;
    };

    class routes_t {
    public:
        routes_t() : total_(), routes_() {};

        void reset(size_t n_routes)
        {
            routes_.clear();
            routes_.reserve(n_routes);
            total_ = 0;
        }

        void add_route (std::vector<location_t>::iterator& current, size_t capacity);

        friend std::ostream& operator<<(std::ostream& out, const routes_t& routes);
    private:
        size_t total_;
        std::vector<route_t> routes_;
    };

    class solution_t {
    public:
        solution_t() : case_no_(), n_cust_(), n_routes_(), name_(), customers_(), routes_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t case_no_;
        size_t n_cust_, n_routes_;
        std::string name_;
        std::vector<location_t> customers_;
        routes_t routes_;
    };

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.customers_.clear();

        if (sol.case_no_ > 0) {
            std::getline(in, sol.name_);
        }

        if (std::getline(in, sol.name_)) {
            in >> sol.n_routes_ >> sol.n_cust_;
            sol.customers_.reserve(sol.n_cust_);
            std::generate_n(std::back_inserter(sol.customers_), sol.n_cust_, [&]() {
                location_t loc;
                in >> loc;
                return loc;
            });
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        if (sol.case_no_ > 1) {
            out << "***********************************" << std::endl;
        }

        out << sol.name_ << std::endl << "Number of Customers: " << std::left << std::setw(10) << sol.n_cust_ << "Number of Routes: " <<
            sol.n_routes_ << std::endl << std::endl;
        out << sol.routes_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        case_no_++;
        std::sort(customers_.begin(), customers_.end());
        auto current = customers_.begin();
        routes_.reset(n_routes_);

        for (size_t idx = 0; idx < n_routes_; idx++) {
            auto capacity = n_cust_ / n_routes_ + (idx < n_cust_ % n_routes_ ? 1 : 0);
            routes_.add_route(current, capacity);
        }

        return *this;
    }

    void routes_t::add_route(std::vector<location_t>::iterator& current, size_t capacity)
    {
        routes_.push_back(route_t());
        routes_.back().populate (current, capacity);
        total_ += routes_.back().cost();
    }

    void route_t::populate(std::vector<location_t>::iterator& current, size_t capacity)
    {
        customers_.reserve(capacity);
        location_t last;

        for (size_t idx = 0; idx < capacity; idx++) {
            cost_ += (std::abs(last.x() - current->x()) + std::abs(last.y() - current->y()));
            last = *current;
            customers_.push_back(&(*current));
            ++current;
        }

        cost_ += std::abs(last.x()) + std::abs(last.y());
    }

    std::ostream& operator<<(std::ostream& out, const routes_t& routes)
    {
        size_t n_route(0);
        std::for_each(routes.routes_.begin(), routes.routes_.end(), [&](const route_t& route) {
            out << "Route ==> " << (++n_route) << std::endl << route << std::endl;
        });
        out << "Total Route length ==> " << routes.total_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const route_t& route)
    {
        std::for_each(route.customers_.begin(), route.customers_.end(), [&](const location_t* customer) {
            out << "Customer: " << customer->name() << std::endl;
        });
        out << "Route length ==> " << route.cost_ << std::endl;
        return out;
    }
}

void U206::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
