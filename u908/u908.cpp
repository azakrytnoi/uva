#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u908.h"
#include "kruskal.h"

#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>
#include <iterator>
#include <numeric>

U908::U908()
{
}

U908::~U908()
{
}

namespace math {

template<>
int Kruskal<int>::operator()(std::vector <Kruskal<int>::Line>& lines)
{
    int total = 0;

    sort(lines.begin(), lines.end(), [](auto a, auto b) -> bool { return a.first < b.first; });
    std::for_each(lines.begin(), lines.end(), [this, &total](auto line) {
        int cost = line.first;
        auto origin = line.second.first;
        auto destination = line.second.second;
        if (!mst_.isSameSet(origin, destination)) {
            total += cost;
            mst_.unionSet(origin, destination);
        }
    });
    return total;
}

}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U908 instance;
    instance();
}
void U908::operator()()
{
    int V;

    auto readLine = []() -> math::Kruskal<int>::Line {
        math::Kruskal<int>::Line line;
        std::cin >> line.second.first >> line.second.second >> line.first;
        return line;
    };
    std::vector <math::Kruskal<int>::Line> lines;
    while (std::cin >> V) {
        math::Kruskal<int> kruskal(V);
        int K, M, total(0);
        lines.clear();
        for (int i = 0; i < V - 1; i++) {
            total += readLine().first;
        }
        std::cout << total << std::endl;
        std::cin >> K;
        std::generate_n(std::back_inserter(lines), K, readLine);
        std::cin >> M;
        std::generate_n(std::back_inserter(lines), M, readLine);
        std::cout << kruskal(lines) << std::endl;
        std::cout << std::endl;
    }
}
