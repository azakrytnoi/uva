#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10012.h"

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
    U10012 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), rad_(), res_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::vector<double_t> rad_;
        double_t res_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max()) {
            in >> sol.N_;
        }

        size_t n(0);

        if (in >> n) {
            sol.rad_.clear();
            sol.rad_.reserve(n);
            std::istream_iterator<double_t> rin(in);
            std::copy_n(rin, n, std::back_inserter(sol.rad_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << std::fixed << std::setprecision(3) << sol.res_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto solve = [&]() -> double_t {
            if (rad_.empty())
            {
                return 0.0;
            } else
            {
                auto res (rad_.front() * 2.0);

                if (rad_.size() > 1) {
                    auto distance = [](double_t a, double_t b) {
                        return std::sqrt((a + b) * (a + b) - (a - b) * (a - b));
                    };
                    std::vector<double_t> temp(rad_.size());
                    temp[0] = rad_[0];

                    for (size_t i = 1; i < rad_.size(); ++i) {
                        temp[i] = temp[i - 1];

                        for (size_t j = 0; j < i; ++j) {
                            temp[i] = std::max(temp[i], std::max(distance(rad_[i], rad_[j]) + temp[j], rad_[i]));
                        }

                        res = std::max(res, temp[i] + rad_[i]);
                    }
                }

                return res;
            }
        };
        std::sort(rad_.begin(), rad_.end());
        res_ = std::numeric_limits<double_t>::max();

        do {
            res_ = std::min(res_, solve());
        } while (std::next_permutation(rad_.begin(), rad_.end()));

        return *this;
    }

}

void U10012::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
