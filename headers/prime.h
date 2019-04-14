/*
 * prime.h
 *
 *  Created on: Mar 24, 2016
 *      Author: alex
 */

#pragma once

#include <bitset>
#include <vector>

namespace math {

    template<uint64_t N>
    class sieve_t {
    public:
        sieve_t() : sieve_(), primes_()
        {
            sieve_.flip();
            sieve_[0] = false;
            sieve_[1] = false;
            primes_.push_back(1);

            for (uint64_t i = 2; i <= N; i++) {
                if (sieve_[i]) {
                    primes_.push_back(i);

                    for (uint64_t j = i * i; j <= N; j += i) {
                        sieve_[j] = false;
                    }
                }
            }

            primes_.shrink_to_fit();
        }

        bool is_prime(uint64_t n) const
        {
            if (n < N) {
                return sieve_[n];
            }

            return false;
        }

        std::vector<uint64_t>::const_iterator begin() const
        {
            return primes_.cbegin();
        }

        std::vector<uint64_t>::const_iterator end() const
        {
            return primes_.cend();
        }

        uint32_t size() const
        {
            return uint32_t(primes_.size());
        }

        uint64_t operator [] (size_t n) const
        {
            return primes_[n];
        }

    private:
        std::bitset < N + 1 > sieve_;
        std::vector<uint64_t> primes_;
    };


}  // namespace math
