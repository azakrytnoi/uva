#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10077.h"

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
	U10077 instance;
	instance();
}

namespace
{
	class solution
	{
	public:
		solution() : n_(), m_(), result_() {}

		friend std::istream& operator >> (std::istream& in, solution& sol);
		friend std::ostream& operator <<(std::ostream& out, const solution& sol);

		operator bool() const { return !(n_ == 1 && m_ == 1); }
		solution& operator()();

	private:
		int32_t m_, n_;
		std::string result_;
	};

	std::istream& operator >> (std::istream& in, solution& sol)
	{
		in >> sol.m_ >> sol.n_;
		return in;
	}

	std::ostream& operator << (std::ostream& out, const solution& sol)
	{
		out << sol.result_;
		return out;
	}

	solution& solution::operator()()
	{
		result_.clear();
		int32_t Nm(1), Nn(1), NLm(0), NLn(1), NRn(0), NRm(1);
		const double value = double(m_) / n_;
		for (; Nm != m_ || Nn != n_; Nm = NLm + NRm, Nn = NLn + NRn)
		{
			if (value > double(Nm) / Nn)
			{
				NLn = Nn; NLm = Nm; result_ += "R";
			}
			else
			{
				NRn = Nn; NRm = Nm; result_ += "L";
			}
		}

		return *this;
	}
}

void U10077::operator()()
{
	solution sol;
	while (std::cin >> sol && sol) {
		std::cout << sol() << std::endl;
	}
}
