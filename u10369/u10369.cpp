#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10369.h"
#include "kruskal.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iosfwd>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U10369 instance;
	instance();
}

namespace {
	double distance(math::Kruskal<double>::Edge& a, math::Kruskal<double>::Edge& b)
	{
		return sqrt((double(a.first) - double(b.first)) * (double(a.first) - double(b.first))
			+ (double(a.second) - double(b.second)) * (double(a.second) - (double(b.second))));
	}

	int S, P;
}

U10369::U10369()
{
}

namespace math {

template<>
double Kruskal<double>::operator()(std::vector <Kruskal<double>::Line>& lines)
{
	double result(0);

	sort(lines.begin(), lines.end(), [](auto a, auto b) -> bool { return a.first < b.first; });
	std::for_each(lines.begin(), lines.end(), [this, &result](auto line) {
		if (mst_.sets == S) return;
		double cost = line.first;
		int origin = line.second.first;
		int destination = line.second.second;
		if (!mst_.isSameSet(origin, destination)) {
			result = cost;
			mst_.unionSet(origin, destination);
		}
	});
	return result;
}

}

void U10369::operator()()
{
	int N;
	std::vector<math::Kruskal<double>::Line> lines;
	std::vector<math::Kruskal<double>::Edge> points;
	std::cin >> N;
	while (N--) {
		std::cin >> S >> P;
		math::Kruskal<double> kruskal(P);
		lines.clear();
		points.clear();
		points.reserve(P);
		std::generate_n(std::back_inserter(points), P, []() -> math::Kruskal<double>::Edge {
			math::Kruskal<double>::Edge point;
			std::cin >> point.first >> point.second;
			return point;
		});
		for (auto i = 0; i < points.size(); ++i) {
			for (auto j = i + 1; j < points.size(); ++j) {
				lines.push_back(std::make_pair(distance(points[i], points[j]), std::make_pair(i, j)));
			}
		}
		std::cout << std::fixed << std::setprecision(2) << kruskal(lines) << std::endl;
	}
}
