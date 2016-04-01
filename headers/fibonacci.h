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
	template<size_t N>
	class fibonacci {
		std::vector<uint64_t> fib_;

	public:
		fibonacci();

		uint64_t operator[](size_t n) const;
		size_t operator()(uint64_t fib) const;
	};

	template<size_t N>
	fibonacci<N>::fibonacci() :
		fib_()
	{
		fib_.reserve(N + 1);
		uint64_t n0[] = { 1, 1 };
		fib_.assign(n0, n0 + 2);
		std::generate_n(std::back_inserter(fib_), N - 1, [&]() -> uint64_t {
			size_t current = fib_.size();
			return fib_[current - 1] + fib_[current - 2];
		});
	}

	template<size_t N>
	uint64_t fibonacci<N>::operator [](size_t n) const
	{
		if (n < N) {
			return fib_[n];
		}
		return 0;
	}

	template<size_t N>
	size_t fibonacci<N>::operator ()(uint64_t fib) const
	{
		auto it = std::find(fib_.begin() + 1, fib_.end(), fib);
		return it == fib_.end() ? std::numeric_limits<size_t>::max() : std::distance(fib_.begin(), it);
	}
}  // namespace math
