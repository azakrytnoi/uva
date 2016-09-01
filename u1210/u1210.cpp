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

#include "prime.h"

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U1210 instance;
	instance();
}

namespace
{
	template<uint64_t N>
	class solution
	{
	public:
		solution() : counts_()
		{
			math::sieve<N> sieve;
			counts_.resize(N + 1);
			for (uint32_t i = 0; i < sieve.size(); i++) {
				uint64_t sum(0);
				for (auto j = sieve.begin() + i; j != sieve.end(); ++j) {
					sum += *j;
					if (sum > N) {
						break;
					}
					++counts_[sum];
				}
			}
		}

		uint32_t operator[] (uint64_t n) const
		{
			return counts_[n];
		}

	private:
		std::vector<uint32_t> counts_;
	};
}

void U1210::operator()() const
{
	solution<10000> sol;
	uint32_t n;
	while ((std::cin >> n) && n > 0) {
		std::cout << sol[n] << std::endl;
	}
}
