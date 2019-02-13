#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u1237.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U1237 instance;
    instance();
}

namespace {

    struct car_t {
        std::string name_;
        uint64_t low_;
        uint64_t high_;

        car_t() : name_(), low_(), high_() {}

        friend std::istream& operator>>(std::istream& in, car_t& car)
        {
            in >> car.name_ >> car.low_ >> car.high_;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const car_t& car)
        {
            out << car.name_ << ": " << car.low_ << "/" << car.high_;
            return out;
        }

        friend bool operator < (const car_t& lhs, const car_t& rhs)
        {
            return lhs.low_ < rhs.low_;
        }
    };

    class solution_t {
    public:
        solution_t() : n_(std::numeric_limits<size_t>::max()), car_db_(), queries_(), results_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return n_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t n_;
        std::vector<std::shared_ptr<car_t>> car_db_;
        std::vector<uint64_t> queries_;
        std::vector<std::string> results_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.car_db_.clear();
        sol.queries_.clear();
        sol.results_.clear();

        if (sol.n_ == std::numeric_limits<size_t>::max()) {
            in >> sol.n_;
        }

        size_t n(0);

        if (in >> n) {
            sol.car_db_.reserve(n);
            std::generate_n(std::back_inserter(sol.car_db_), n, [&]() {
                auto car (std::make_shared<car_t>());
                in >> *car;
                return car;
            });
            in >> n;
            sol.queries_.reserve(n);
            sol.results_.reserve(n);
            std::istream_iterator<uint64_t> iq(in);
            std::copy_n(iq, n, std::back_inserter(sol.queries_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<std::string> rout(out, "\n");
        std::copy(sol.results_.begin(), sol.results_.end(), rout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<std::shared_ptr<car_t>> tmp;
        tmp.reserve(car_db_.size());

        for (auto qry : queries_) {
            tmp.clear();
            std::copy_if(car_db_.begin(), car_db_.end(), std::back_inserter(tmp), [&](const std::shared_ptr<car_t>& car) {
                return car->low_ <= qry && car->high_ > qry;
            });

            if (tmp.size() == 1) {
                results_.push_back(tmp[0]->name_);
            } else {
                results_.push_back("UNDETERMINED");
            }
        }

        return *this;
    }

}

void U1237::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
