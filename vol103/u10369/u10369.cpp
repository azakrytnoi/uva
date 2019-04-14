﻿
#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10369.h"
#include <kruskal.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iosfwd>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10369 instance;
    instance();
}

namespace {
    double distance(math::kruskal_t<double>::edge_t& a, math::kruskal_t<double>::edge_t& b)
    {
        return std::sqrt((double(a.first) - double(b.first)) * (double(a.first) - double(b.first))
                         + (double(a.second) - double(b.second)) * (double(a.second) - (double(b.second))));
    }

    size_t S, P;
}

U10369::U10369()
{}

namespace math {

    template<>
    double kruskal_t<double>::operator()(std::vector <kruskal_t<double>::line_t>& lines)
    {
        double result(0);

        sort(lines.begin(), lines.end(), [](auto a, auto b) -> bool { return a.first < b.first; });
        std::for_each(lines.begin(), lines.end(), [this, &result](auto line) {
            if (mst_.sets_ == S) {
                return;
            }

            double cost = line.first;
            auto origin = line.second.first;
            auto destination = line.second.second;

            if (!mst_.isSameSet(origin, destination)) {
                result = cost;
                mst_.unionSet(origin, destination);
            }
        });
        return result;
    }

}

void U10369::operator()() const
{
    int N;
    std::vector<math::kruskal_t<double>::line_t> lines;
    std::vector<math::kruskal_t<double>::edge_t> points;
    std::cin >> N;

    while (N--) {
        std::cin >> S >> P;
        math::kruskal_t<double> kruskal(P);
        lines.clear();
        points.clear();
        points.reserve(P);
        std::generate_n(std::back_inserter(points), P, []() -> math::kruskal_t<double>::edge_t {
            math::kruskal_t<double>::edge_t point;
            std::cin >> point.first >> point.second;
            return point; });

        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                lines.push_back(std::make_pair(distance(points[i], points[j]), std::make_pair(i, j)));
            }
        }

        std::cout << std::fixed << std::setprecision(2) << kruskal(lines) << std::endl;
    }
}
