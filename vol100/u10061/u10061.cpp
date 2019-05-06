#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10061.h"

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
    U10061 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(), B_(), zero_(), digits_(), dp_()
        {
            dp_.reserve(100000);
            std::generate_n(std::back_inserter(dp_), 100000, [&]()
            {
                if (dp_.empty())
                {
                    return 0.0;
                }

                auto res(dp_.back() + std::log(dp_.size()));
                return res;
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
        uint32_t N_, B_;
        uint32_t zero_, digits_;
        std::vector<double_t> dp_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.N_ >> sol.B_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.zero_ << ' ' << sol.digits_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto zero = [&]() -> uint32_t
        {
            auto res(std::numeric_limits<uint32_t>::max());
            auto B(B_);

            for (auto i = 2u; i <= B_; ++i)
            {
                if (B % i == 0)
                {
                    auto a (0u);

                    while (B % i == 0)
                    {
                        ++a;
                        B /= i;
                    }

                    auto b(0u), tmp(N_);

                    while (tmp)
                    {
                        b += tmp / i;
                        tmp /= i;
                    }

                    res = std::min(res, b / a);
                }
            }

            if (res == std::numeric_limits<uint32_t>::max())
            {
                return 0u;
            }

            return res;
        };
        zero_ = zero();
        auto digits = [&]() ->uint32_t
        {
            if (N_ > 100000)
            {
                return static_cast<uint32_t>(((std::log(std::sqrt(2 * M_PI * N_)) + N_ * std::log(N_ / M_E)) / std::log(B_)) + 1);
            }

            return std::ceil(dp_[N_] / std::log(B_) + 1e-10);
        };
        digits_ = digits();
        return *this;
    }

}

void U10061::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
