#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u184.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <list>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U184 instance;
	instance();
}

namespace {
	class solution
	{
	public:
		solution() : points_(), lines_()
		{
			points_.reserve(300);
		}

		friend
			std::istream& operator >> (std::istream& in, solution& sol) {
			sol.points_.clear();
			while (in)
			{
				std::pair<uint16_t, uint16_t> point;
				in >> point.first >> point.second;
				if (point.first == 0 && point.second == 0) break;
				sol.points_.push_back(point);
			}
			return in;
		}

		friend
			std::ostream& operator << (std::ostream& out, const solution& sol) {
			std::for_each(sol.lines_.begin(), sol.lines_.end(), [&](auto line) {
				std::for_each(line.begin(), line.end(), [&](auto point) {
					out << "(" << std::setw(4) << point.first << "," << std::setw(4) << point.second << ")";
				});
				out << std::endl;
			});
			return out;
		}

		bool solve();
		bool empty() const { return points_.empty(); }

	private:
		std::vector<std::pair<uint16_t, uint16_t>> points_;
		std::list<std::list<std::pair<uint16_t, uint16_t>>> lines_;
	};

	bool solution::solve() {
		lines_.clear();
		for (auto pa = points_.begin(); pa != points_.end() - 2; ++pa) {
			std::list<std::pair<uint16_t, uint16_t>> line;
			for (auto pb = pa + 1; pb != points_.end() - 1; ++pb) {
				if (std::find_if(line.begin(), line.end(), [&](std::pair<uint16_t, uint16_t>& p) -> bool {
					return (p.first == pb->first)
						&& (p.second == pb->second);
				}) == line.end()) {
					for (auto pc = pb + 1; pc != points_.end(); ++pc) {
						if (pa->first * (pb->second - pc->second) + pb->first * (pc->second - pa->second) + pc->first * (pa->second - pb->second) == 0) {
							if (line.empty()) {
								line.push_back(*pa);
								line.push_back(*pb);
							}
							line.push_back(*pc);
						}
					}
				}
			}
			if (!line.empty()) {
				lines_.push_back(line);
			}
		}
		return !lines_.empty();
	}
}

void U184::operator()()
{
	solution sol;
	while (std::cin >> sol && !sol.empty())
	{
		if (sol.solve()) {
			std::cout << "The following lines were found:" << std::endl;
			std::cout << sol;
		}
		else
		{
			std::cout << "No lines were found" << std::endl;
		}
	}
}