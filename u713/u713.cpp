#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u713.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <string>

U713::U713()
{
}

namespace {
	class solver {
		std::string a_, b_;
	public:

		solver() : a_(), b_() {}

		friend
			std::istream& operator >> (std::istream& in, solver& s)
		{
			in >> s.a_;
			in >> s.b_;
			return in;
		}

		friend
			std::ostream& operator << (std::ostream& out, const solver& s)
		{
			int carry(0);
			std::stringstream ss;
			auto ita = s.a_.begin(), itb = s.b_.begin();
			for (; ita != s.a_.end() && itb != s.b_.end(); ) {
				int res = (*(ita++) - '0') + (*(itb++) - '0') + carry;
				ss << (res % 10);
				carry = res / 10;
			}
			while (ita != s.a_.end()) {
				int res = (*(ita++) - '0') + carry;
				ss << (res % 10);
				carry = res / 10;
			}
			while (itb != s.b_.end()) {
				int res = (*(itb++) - '0') + carry;
				ss << (res % 10);
				carry = res / 10;
			}
			if (carry != 0) {
				ss << carry;
			}
			std::string res(ss.str());
			res.erase(0, res.find_first_not_of('0'));
			out << res;
			return out;
		}
	};
}

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U713 instance;
	instance();
}
void U713::operator()()
{
	int N;
	std::cin >> N;
	solver s;
	while (N--) {
		std::cin >> s;
		std::cout << s << std::endl;
	}
}