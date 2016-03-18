#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u1210.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <bitset>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U1210 instance;
    instance();
}

namespace {
template<uint32_t N>
class sieve {
public:
    sieve() : sieve_(), primes_()
    {
        sieve_.flip();
        sieve_[0] = false;
        sieve_[1] = false;
        for (uint32_t i = 2; i <= N; i++) {
            if (sieve_[i]) {
                primes_.push_back(i);
                for (uint32_t j = i * i; j <= N; j += i) {
                    sieve_[j] = false;
                }
            }
        }
        primes_.shrink_to_fit();
    }

    bool is_primme(uint32_t n) const
    {
        if (n < N) {
            return sieve_[n];
        }
        return false;
    }

    std::vector<uint32_t>::const_iterator begin() const
    {
        return primes_.cbegin();
    }

    std::vector<uint32_t>::const_iterator end() const
    {
        return primes_.cend();
    }

    uint32_t size() const
    {
        return uint32_t(primes_.size());
    }

private:
    std::bitset<N + 1> sieve_;
    std::vector<uint32_t> primes_;
};

template<uint32_t N>
class solution {
public:
    solution() : sieve_(), counts_()
    {
        counts_.resize(N + 1);
        for (uint32_t i = 0; i < sieve_.size(); i++) {
            uint32_t sum(0);
            for (auto j = sieve_.begin() + i; j != sieve_.end(); ++j) {
                sum += *j;
                if (sum > N) break;
                ++counts_[sum];
            }
        }
    }

    uint32_t operator[] (uint32_t n) const
    {
        return counts_[n];
    }

private:
    sieve<N> sieve_;
    std::vector<uint32_t> counts_;
};
}

void U1210::operator()()
{
    solution<10000> sol;
    uint32_t n;
    while ((std::cin >> n) && n > 0) {
        std::cout << sol[n] << std::endl;
    }
}