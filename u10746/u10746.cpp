#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10746.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <bitset>
#include <queue>
#include <cstring>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10746 instance;
    instance();
}

namespace {
    template<size_t N>
    class mcmf {
        std::vector<int> prev_;
        std::vector<int> last_;
        std::vector<int> to_;
        std::vector<int> next_;
        std::vector<int> cap_;
        std::vector<double> cost_;
        std::vector<double> dist_;
        std::vector<double> pot_;

        int v_;
        int idx_;
    public:

        mcmf() :
            prev_(), last_(), to_(), next_(), cap_(), cost_(), dist_(), pot_(), v_(0), idx_(0)
        {
            last_.reserve(N);
            to_.reserve(N * 20);
            next_.reserve(N * 20);
            cap_.reserve(N * 20);
            cost_.reserve(N * 20);
            dist_.resize(N);
            pot_.reserve(N);
            prev_.resize(N);
        }

        void init(int v)
        {
            v_ = v;
            idx_ = 0;
            last_.clear();
            std::generate_n(std::back_inserter(last_), N, []() {
                return -1;
            });
            to_.clear();
            to_.resize(to_.capacity());
            cap_.clear();
            cap_.resize(cap_.capacity());
            cost_.clear();
            cost_.resize(cost_.capacity());
            next_.clear();
            next_.resize(next_.capacity());
        }

        void add_edge(int u, int v, int capacity, double cost)
        {
            to_[idx_] = v;
            cap_[idx_] = capacity;
            cost_[idx_] = cost;
            next_[idx_] = last_[u];
            last_[u] = idx_++;
            to_[idx_] = u;
            cap_[idx_] = 0;
            cost_[idx_] = -cost;
            next_[idx_] = last_[v];
            last_[v] = idx_++;
        }

        double operator()(int start, int end)
        {
            int val(0);
            double cost(0);
            pot_.clear();
            pot_.resize(N);

            while (true) {
                std::generate_n(prev_.begin(), N, []() {
                    return -1;
                });
                std::bitset<N> visited;
                std::generate_n(dist_.begin(), v_, []() {
                    return INF;
                });

                std::priority_queue<std::pair<double, int>> work_queue;
                work_queue.push(std::make_pair(0, start));
                dist_[start] = prev_[start] = 0;

                while (!work_queue.empty()) {
                    int idx = work_queue.top().second;
                    work_queue.pop();

                    if (visited[idx]) {
                        continue;
                    }

                    visited[idx] = true;

                    for (int edge = last_[idx]; edge != -1; edge = next_[edge]) {
                        if (cap_[edge] <= 0) {
                            continue;
                        }

                        double new_dist = dist_[idx] + cost_[edge] + pot_[idx] - pot_[to_[edge]];

                        if (new_dist < dist_[to_[edge]]) {
                            dist_[to_[edge]] = new_dist;
                            prev_[to_[edge]] = edge;
                            work_queue.push(std::make_pair(-new_dist, to_[edge]));
                        }
                    }
                }

                if (prev_[end] == -1) {
                    break;
                }

                int factor = cap_[prev_[end]];

                for (int i = end; i != start; i = to_[prev_[i] ^ 1]) {
                    factor = std::min(factor, cap_[prev_[i]]);
                }

                for (int i = end; i != start; i = to_[prev_[i] ^ 1]) {
                    cap_[prev_[i]] -= factor;
                    cap_[prev_[i] ^ 1] += factor;
                }

                val += factor;
                cost += factor * (dist_[end] - pot_[start] + pot_[end]);

                for (int i = 0; i < v_; ++i) {
                    if (prev_[i] != -1) {
                        pot_[i] += dist_[i];
                    }
                }
            }

            return cost;
        }

    private:
        static constexpr double INF = 1e10;
    };
} // namespace

void U10746::operator()() const
{
    int n, m;
    mcmf<21 * 2> solver;

    while ((std::cin >> n >> m) && (n != 0 && m != 0)) {
        solver.init(n + m + 2);

        for (int i = 0; i < n; ++i) {
            solver.add_edge(0, 1 + i, 1, 0);
        }

        for (int i = 0; i < m; ++i) {
            solver.add_edge(1 + n + i, 1 + n + m, 1, 0);
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                double cost;
                std::cin >> cost;
                solver.add_edge(1 + i, 1 + n + j, 1, cost);
            }
        }

        double cost = solver(0, 1 + n + m);
        cost /= n;
        cost = floor(cost * 100.0 + 0.5 + 1e-9) / 100.0;
        std::cout << std::fixed << std::setprecision(2) << cost << std::endl;
    }
}
