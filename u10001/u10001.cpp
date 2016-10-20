#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10001.h"

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
	U10001 instance;
	instance();
}

namespace
{
	class solution
	{
		uint32_t ident_;
		uint32_t lattice_;
		int32_t n_;
	public:
		solution() : ident_(), lattice_(), n_() {}

		friend std::istream& operator >> (std::istream& in, solution& sol);
		friend std::ostream& operator <<(std::ostream& out, const solution& sol);

		bool operator()(int32_t li, int32_t fcr, int32_t plc) const;

	private:
	};

	std::istream& operator >> (std::istream& in, solution& sol)
	{
		std::string s;
		in >> sol.ident_ >> sol.n_ >> s;
		if (in) {
			sol.lattice_ = 0;
			for (int32_t i = 0; i < sol.n_; i++)
			{
				if (s[i] == '1')
				{
					sol.lattice_ |= static_cast<uint32_t>(1) << (sol.n_ - i - 1);
				}
			}
		}
		return in;
	}

	std::ostream& operator << (std::ostream& out, const solution& sol)
	{
		out << (sol(0, -1, -1) ? "REACHABLE" : "GARDEN OF EDEN");
		return out;
	}

	bool solution::operator()(int32_t li, int32_t fcr, int32_t plc) const
	{
		int id = (lattice_ & (static_cast<unsigned int>(1) << li))
			/* the value of current cell is 1 */ ?
			ident_ : ~ident_;
		for (int i = 0; i < 4; i++, id >>= 2)
			if (id & 3) {
				int lc_0 = -1, lc_1 = -1;
				// left, cell, and right bits whose new state is equal
				// to the current value
				if (id & 1)
					lc_0 = i * 2;
				if (id & 2)
					lc_1 = i * 2 + 1;
				if (!li) { // for the first cell
					// pass the cell and right bits of the first cell
					if (id & 1 &&
						(*this)(li + 1, lc_0 & 3, i) ||
						id & 2 &&
						(*this)(li + 1, lc_1 & 3, i))
						return true;
				}
				else if (li == n_ - 1) { // for the last cell
					if (lc_0 != -1 && (lc_0 >> 1) == fcr &&
						(lc_0 & 3) == plc ||
						lc_1 != -1 && (lc_1 >> 1) == fcr && (lc_1 & 3) == plc)
						return true;
				}
				else if (lc_0 != -1 && (lc_0 & 3) == plc ||
					lc_1 != -1 && (lc_1 & 3) == plc) {
					if ((*this)(li + 1, fcr, i))
						// cell and right bits of the current cell are equal
						// to the left ant cell bits of the previous cell
						return true;
				}
			}
		return false;
	}
}

void U10001::operator()() const
{
	solution sol;
	while (std::cin >> sol) {
		std::cout << sol << std::endl;
	}
}