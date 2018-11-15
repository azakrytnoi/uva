#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10160.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <functional>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10160 instance;
    instance();
}

namespace {
class solution {
    int32_t n_, m_;
    int32_t min_covered_;
    std::vector<uint64_t> adjacency_;
public:
    solution() : n_(), m_(), min_covered_(), adjacency_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return !(n_ == 0 && m_ == 0);
    }
    solution& operator()();

private:
    static void dominating_set(const std::vector<int32_t>& vertices,
                               int32_t n /* number of vertices */, int32_t iv /* index to the vector of vertices */,
                               int32_t v /* number of vertices used so far */,
                               const std::vector<uint64_t>& adjacency,
                               const std::vector<uint64_t>& coverable,
                               const uint64_t all_covered, uint64_t covered,
                               int32_t& min_covered);
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.n_ >> sol.m_;

    if (sol) {
        sol.adjacency_.clear();
        sol.adjacency_.resize(sol.n_);
        int32_t cnt(sol.m_);

        while (cnt--) {
            int32_t u, v;
            in >> u >> v;
            --u;
            --v;
            sol.adjacency_[u] |= static_cast<uint64_t>(1) << v;
            sol.adjacency_[v] |= static_cast<uint64_t>(1) << u;
        }

        sol.min_covered_ = 0;
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.min_covered_;
    return out;
}

solution& solution::operator()()
{
    std::multimap<int32_t, int32_t, std::greater<int32_t>> degrees;

    // value is a vertex, key is its degree
    {
        int32_t i(0);
        std::for_each(adjacency_.begin(), adjacency_.end(), [&](const uint64_t item) {
            int32_t degree(0);
            uint64_t mask(1);

            for (int32_t j = 0; j < n_; j++, mask <<= 1) {
                if (item & mask) {
                    degree++;
                }
            }

            degrees.insert(std::make_pair(degree, i++));
        });
    }

    // vector of vertices in descending order of their degrees
    std::vector<int32_t> vertices;
    vertices.reserve(n_);
    std::transform(degrees.begin(), degrees.end(), std::back_inserter(vertices), [](const std::pair<int32_t, int32_t>& p) {
        return p.second;
    });

    std::vector<uint64_t> coverable(n_);

    // coverable[i] is the bitmap covered by the set of vertices from
    // vertices[i] - vertices[n - 1]
    uint64_t c(0);

    for (int32_t iv = n_ - 1; iv >= 0; iv--) {
        int32_t i = vertices[iv];
        c |= adjacency_[i] | (static_cast<uint64_t>(1) << i);
        coverable[iv] = c;
    }

    uint64_t all_covered = (static_cast<uint64_t>(1) << n_) - 1, covered = 0;
    min_covered_ = n_;
    dominating_set(vertices, n_, 0, 0, adjacency_, coverable, all_covered, covered, min_covered_);
    return *this;
}

void solution::dominating_set(const std::vector<int32_t>& vertices, int32_t n, int32_t iv, int32_t v, const std::vector<uint64_t>& adjacency,
                              const std::vector<uint64_t>& coverable, const uint64_t all_covered, uint64_t covered, int32_t& min_covered)
{
    if (min_covered <= v + 1 || iv == n) {
        return; // no need to further search
    }

    if ((covered | coverable[iv]) != all_covered) {
        return;
    }

    int32_t i = vertices[iv];
    uint64_t current = static_cast<uint64_t>(1) << i;
    uint64_t c = covered | adjacency[i] | current;

    if (c == all_covered) {
        min_covered = v + 1;
        return;
    }

    dominating_set(vertices, n, iv + 1, v + 1, adjacency, coverable, all_covered, c, min_covered);
    dominating_set(vertices, n, iv + 1, v, adjacency, coverable, all_covered, covered, min_covered);
}
}

void U10160::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
