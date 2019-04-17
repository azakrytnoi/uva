/*
 * fibonacci.h
 *
 *  Created on: Mar 18, 2016
 *      Author: alex
 */

#pragma once

#include <vector>
#include <iterator>
#include <limits>

namespace math {
    template<size_t N, typename fib_t>
    class fibonacci_t {
        std::vector<fib_t> fib_;

    public:
        fibonacci_t();

        fib_t operator[](size_t n) const;
        size_t operator()(const fib_t& fib) const;
    };

    template<size_t N, typename fib_t>
    fibonacci_t<N, fib_t>::fibonacci_t() :
        fib_()
    {
        fib_.reserve(N + 1);
        fib_t n0[] = { 1, 1 };
        fib_.assign(n0, n0 + 2);
        std::generate_n(std::back_inserter(fib_), N - 1, [&]() -> fib_t
        {
            size_t current = fib_.size();
            return fib_[current - 1] + fib_[current - 2];
        });
    }

    template<size_t N, typename fib_t>
    fib_t fibonacci_t<N, fib_t>::operator [](size_t n) const
    {
        if (n < N)
        {
            return fib_[n];
        }

        return 0;
    }

    template<size_t N, typename fib_t>
    size_t fibonacci_t<N, fib_t>::operator ()(const fib_t& fib) const
    {
        typedef typename std::vector<fib_t>::const_iterator  iterator_t;
        auto b_search = [&](iterator_t lower, iterator_t upper, const fib_t & val) -> iterator_t
        {
            auto mid (lower);
            std::advance(mid, std::distance(lower, upper) / 2);

            while (val != *mid && std::distance(lower, upper) > 0)
            {
                if (val < *mid)
                {
                    upper = mid - 1;
                }
                else
                {
                    lower = mid + 1;
                }

                mid = lower;
                std::advance(mid, std::distance(lower, upper) / 2);
            }

            if (std::distance(lower, upper) >= 0)
            {
                return mid;
            }

            return fib_.end();
        };
        auto it = b_search(fib_.begin() + 1, fib_.end(), fib);
        return it == fib_.end() ? std::numeric_limits<size_t>::max() : std::distance(fib_.begin(), it);
    }
}  // namespace math
