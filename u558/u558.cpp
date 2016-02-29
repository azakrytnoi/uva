#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u558.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

U558::U558()
{
}


U558::~U558()
{
}

namespace
{
struct edge {
    int start, end, weight;
};
}

void U558::operator()()
{
    int N;
    std::cin >> N;
    std::vector<edge> wormholes;
    std::vector<int> distances;
    while (N--) {
        wormholes.clear();
        distances.clear();
        int n_systems, n_wormholes;
        std::cin >> n_systems >> n_wormholes;

        wormholes.reserve(n_wormholes);
        std::generate_n(std::back_inserter(wormholes), n_wormholes, []() {
            edge e;
            std::cin >> e.start >> e.end >> e.weight;
            return e;
        });

        distances.reserve(n_systems + 1);
        std::generate_n(std::back_inserter(distances), n_systems, []() {
            return U558::Infinity;
        });
        distances[0] = 0;

        for (int i = 0; i < n_systems - 1; i++) {
            for (int j = 0; j < n_wormholes; j++) {
                if (distances[wormholes[j].start] + wormholes[j].weight < distances[wormholes[j].end])
                    distances[wormholes[j].end] = distances[wormholes[j].start] + wormholes[j].weight;
            }
        }

        bool have_negative_cycle = false;

        for (int j = 0; !have_negative_cycle && j < n_wormholes; j++) {
            if (distances[wormholes[j].start] + wormholes[j].weight < distances[wormholes[j].end])
                have_negative_cycle = true;
        }

        std::cout << (have_negative_cycle ? "possible" : "not possible") << std::endl;
    }
}