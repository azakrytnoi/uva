#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11727.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
	U11727 instance;
	instance();
}

namespace {
	class solution {
		std::vector<int32_t> data_;
	public:
		solution() : data_()
		{
			data_.reserve(3);
		}

		friend std::istream& operator >> (std::istream& in, solution& sol);
		friend std::ostream& operator <<(std::ostream& out, const solution& sol);

		operator bool() const
		{
			return true;
		}
		solution& operator()();

	private:
	};

	std::istream& operator >> (std::istream& in, solution& sol)
	{
		sol.data_.clear();
		std::generate_n(std::back_inserter(sol.data_), 3, [&]() {
			int32_t tmp;
			in >> tmp;
			return tmp;
		});
		return in;
	}

	std::ostream& operator << (std::ostream& out, const solution& sol)
	{
		out << sol.data_[1];
		return out;
	}

	solution& solution::operator()()
	{
		std::sort(data_.begin(), data_.end());
		return *this;
	}
}

void U11727::operator()() const
{
	int32_t N;
	std::cin >> N;
	solution sol;
	int32_t caseNo(0);

	while (N--) {
		std::cin >> sol;
		std::cout << "Case " << (++caseNo) << ": " << sol() << std::endl;
	}
}
