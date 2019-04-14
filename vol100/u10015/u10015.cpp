#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10015.h"

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

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10015 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : primes_(), n_(), res_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return n_ > 0;
        }
        solution_t& operator()();

    private:
        math::sieve_t<40000> primes_;
        uint64_t n_;
        uint64_t res_;

        uint64_t f(uint64_t n, uint64_t k)
        {
            if (n == 1) {
                return 1;
            }

            return ((f(n - 1, k) + k - 1) % n) + 1;
        };
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.res_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        res_ = f(n_, primes_[n_]);
        return *this;
    }

}

void U10015::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
