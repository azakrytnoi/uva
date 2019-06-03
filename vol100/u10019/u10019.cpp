#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10019.h"

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
    U10019 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), number_(), b10_(), b16_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::string number_;
        size_t b10_, b16_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        in >> sol.number_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.b10_ << ' ' << sol.b16_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto countBits = [](uint64_t num)
        {
            size_t counter(0);

            while (num != 0)
            {
                counter += num & 0x01;
                num >>= 1;
            }

            return counter;
        };
        std::stringstream _10in(number_);
        uint64_t num;
        _10in >> num;
        b10_ = countBits(num);
        std::stringstream _16n(number_);
        _16n >> std::hex >> num;
        b16_ = countBits(num);
        return *this;
    }

}

void U10019::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
