#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u256.h"

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
    U256 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : qn_(), result_()
        {
            qn_.reserve(10000);
            std::generate_n(std::back_inserter(qn_), 10000, [&]()
            {
                return qn_.size() * qn_.size();
            });
        }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        std::vector<uint32_t> qn_;
        static uint16_t n_;

        struct formatted_n
        {
            uint32_t val_;
            size_t size_;

            formatted_n(uint32_t n) : val_(n), size_(n_) {}

            friend std::ostream& operator << (std::ostream& out, const formatted_n& n)
            {
                out << std::setfill('0') << std::setw(n.size_) << n.val_;
                return out;
            }
        };

        std::vector<formatted_n> result_;
    };

    uint16_t solution_t::n_ (0);

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<solution_t::formatted_n> iout(out, "\n");
        std::copy(sol.result_.begin(), sol.result_.end(), iout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        const uint32_t upper_value (std::pow(10, n_));
        const uint32_t mid (std::pow(10, n_ / 2));
        auto ubound = std::upper_bound(qn_.begin(), qn_.end(), upper_value);

        if (ubound != qn_.end())
        {
            while (*ubound > upper_value)
            {
                --ubound;
            }
        }

        result_.clear();
        result_.reserve(std::distance(qn_.begin(), ubound));
        std::copy_if(qn_.begin(), ubound, std::back_inserter(result_), [&](auto qn)
        {
            uint16_t upper = qn / mid;
            uint16_t lower = qn % mid;
            return uint32_t((upper + lower) * (upper + lower)) == qn;
        });
        return *this;
    }

}

void U256::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol();
    }
}
