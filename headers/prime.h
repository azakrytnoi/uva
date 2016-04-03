/*
 * prime.h
 *
 *  Created on: Mar 24, 2016
 *      Author: alex
 */

#pragma once

#include <bitset>
#include <vector>

namespace math
{

template<uint32_t N>
class sieve
{
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

    uint32_t operator [] (size_t n) const
    {
        return primes_[n];
    }

private:
    std::bitset<N + 1> sieve_;
    std::vector<uint32_t> primes_;
};


}  // namespace math
