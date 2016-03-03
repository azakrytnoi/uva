#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u412.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <iomanip>

#include "gcd.h"

U412::U412() {}

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U412 instance;
	instance();
}
void U412::operator()()
{
	uint32_t N;
	std::vector<uint32_t> series;
	while (std::cin >> N && N > 0) {
		series.clear();
		series.reserve(N);
		std::istream_iterator<uint32_t> iit(std::cin);
		std::copy_n(iit, N, std::back_inserter(series));
		uint32_t count(0);
		for (size_t i = 0; i < N - 1; i++) {
			for (size_t j = i + 1; j < N; j++) {
				if (math::gcd(series[i], series[j]) == 1) {
					count++;
				}
			}
		}
		if (count == 0) {
			std::cout << "No estimate for this data set." << std::endl;
		}
		else {
			uint32_t total(N * (N - 1) / 2);
			std::cout << std::fixed << std::setprecision(6) << std::sqrt(6.0 * total / count) << std::endl;
		}
	}
}