#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u446.h"

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
    U446 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_case_(std::numeric_limits<size_t>::max()), hex_a_(), hex_b_(), op_(), a_(), b_(), sum_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return n_case_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t n_case_;
        std::string hex_a_, hex_b_, op_;
        uint16_t a_, b_;
        uint16_t sum_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.n_case_ == std::numeric_limits<size_t>::max()) {
            in >> sol.n_case_;
        }

        in >> sol.hex_a_ >> sol.op_ >> sol.hex_b_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        auto to_binstr = [](uint16_t n) -> std::string {
            std::string out (13, '0');
            auto pout (out.rbegin());

            while (n > 0)
            {
                *(pout++) = (n & 1) == 1 ? '1' : '0';
                n >>= 1;
            }

            return out;
        };
        out << to_binstr(sol.a_) << " " << sol.op_ << " " << to_binstr(sol.b_) << " = " << sol.sum_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto from_hexstr = [](const std::string & src) -> uint16_t {
            uint16_t result(std::accumulate(src.begin(), src.end(), 0, [](uint16_t prev, char ch)
            {
                prev <<= 4;
                return ch >= 'A' ? prev + 10 + ch - 'A' : prev + ch - '0';
            }));
            return result;
        };
        a_ = from_hexstr(hex_a_);
        b_ = from_hexstr(hex_b_);

        switch (op_[0]) {
        case '+':
            sum_ = a_ + b_;
            break;

        case '-':
            sum_ = a_ - b_;
            break;

        default:
            break;
        }

        return *this;
    }

}

void U446::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
