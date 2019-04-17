#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u344.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U344 instance;
    instance();
}

namespace {

    class solution {
        std::vector<uint32_t> i_0_ = { 0, 1, 2, 3, 1, 0, 1, 2, 3, 1 };
        std::vector<uint32_t> v_0_ = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 };
        std::vector<uint32_t> x_0_ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

        std::vector<uint32_t> x_10_ = { 0, 1, 2, 3, 1, 0, 1, 2, 3, 1, 0 };
        std::vector<uint32_t> l_10_ = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 };
        std::vector<uint32_t> c_10_ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };

        std::vector<uint32_t> i_, v_, x_, l_, c_;

        uint32_t n_;
    public:
        solution() : i_(101), v_(101), x_(101), l_(101), c_(101), n_(0)
        {
            for (size_t i = 1; i <= 100; i++)
            {
                i_[i] += i_[i - 1] + i_0_[i % 10];
                v_[i] += v_[i - 1] + v_0_[i % 10];
                x_[i] += x_[i - 1] + x_0_[i % 10] + x_10_[i / 10];
                l_[i] += l_[i - 1] + l_10_[i / 10];
                c_[i] += c_[i - 1] + c_10_[i / 10];
            }
        }

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            in >> sol.n_;
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol)
        {
            out << sol.n_ << ": " << sol.i_[sol.n_] << " i, " << sol.v_[sol.n_] << " v, "
                << sol.x_[sol.n_] << " x, " << sol.l_[sol.n_] << " l, " << sol.c_[sol.n_] << " c";
            return out;
        }

        operator bool () const
        {
            return n_ != 0;
        }

    private:
    };

}

void U344::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol << std::endl;
    }
}
