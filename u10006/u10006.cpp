#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10006.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>
#include <map>

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10006 instance;
    instance();
}

namespace
{

class solution
{
    std::map<uint32_t, bool> carmichael_;
    uint32_t n_;
public:
    solution() : carmichael_(), n_(0)
    {
        std::unique_ptr<math::sieve<10000000>> sieve (new math::sieve<10000000>);
        std::clog << *(sieve->end() - 1) << std::endl;
        for (uint32_t i = 2; i <= 65000; i++) {
            if (!sieve->is_prime(i) && is_carmichael(i)) {
                carmichael_[i] = true;
            }
        }
    }

    friend std::istream& operator >>(std::istream& in, solution& sol)
    {
        in >> sol.n_;
        return in;
    }
    friend std::ostream& operator <<(std::ostream& out, solution& sol)
    {
        if (sol.carmichael_[sol.n_]) {
            out << "The number " << sol.n_ << " is a Carmichael number.";
        } else {
            out << sol.n_ << " is normal.";
        }
        return out;
    }

    operator bool() const
    {
        return n_ != 0;
    }

private:
    bool is_carmichael(uint32_t n)
    {
        for(uint32_t i = 2; i < n; i++) {
            if(mod_pow(i, n, n) != i) {
                return false;
            }
        }
        return true;
    }

    static uint32_t mod_pow(uint64_t base, uint32_t exp, uint32_t mod)
    {
        uint32_t result = 1;
        while(exp > 0) {
            if(exp % 2 == 1) {
                result = (result * base) % mod;
            }
            exp = exp >> 1;
            base = (base * base) % mod;
        }
        return result;
    }
};

}

void U10006::operator()() const
{
    solution sol;
    while (std::cin >> sol && sol) {
        std::cout << sol << std::endl;
    }
}
