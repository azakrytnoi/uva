#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10014.h"

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
    U10014 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), a0_(), an1_(), c_(), res_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        double_t a0_, an1_;
        std::vector<double_t> c_;
        double_t res_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max()) {
            in >> sol.N_;
        }

        size_t n(0);

        if (in >> n) {
            in >> sol.a0_ >> sol.an1_;
            std::istream_iterator<double_t> din(in);
            sol.c_.clear();
            sol.c_.reserve(n);
            std::copy_n(din, n, std::back_inserter(sol.c_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << std::fixed << std::setprecision(2) << sol.res_ << std::endl;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto n = c_.size();
        res_ = a0_ * n + an1_;

        for (size_t i = n; i >= 1; --i) {
            res_ -= 2 * (n - i + 1) * c_[i - 1];
        }

        res_ /= double_t(n + 1);

        return *this;
    }

}

void U10014::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
