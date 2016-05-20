#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10235.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <cstdlib>

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10235 instance;
    instance();
}

namespace
{

template<size_t N>
class solution
{
public:
    solution() : primes_()
    {
        math::sieve<N> sieve;
        std::for_each(sieve.begin(), sieve.end(), [&](auto prime) {
            primes_[prime] = true;
        });
    }

    std::string operator() (int n, int m)
    {
        if (primes_[n]) {
            if (primes_[m] && m != n) {
                return " is emirp.";
            }
            return " is prime.";
        }
        return " is not prime.";
    }

private:
    std::map<int, bool> primes_;
};

}  // namespace

void U10235::operator()() const
{
    solution<1000001> sol;
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        int n = std::atoi(line.c_str());
        std::reverse(line.begin(), line.end());
        int m = std::atoi(line.c_str());
        std::cout << n << sol(n, m) << std::endl;
    }
}
