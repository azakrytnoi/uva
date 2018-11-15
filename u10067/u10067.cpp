#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10067.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <queue>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10067 instance;
    instance();
}

namespace {
class solution {
    int32_t start_, target_;
    std::vector<bool> forbid_;
    int32_t distance_;
public:
    solution() : start_(), target_(), forbid_(), distance_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return true;
    }
    solution& operator()();

private:
    static int32_t read(std::istream& in);
    int32_t bfs();
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.forbid_.clear();
    sol.start_ = sol.read(in);
    sol.target_ = sol.read(in);
    int32_t n;
    in >> n;
    sol.forbid_.resize(10000);

    for (int32_t i = 0; i < n; i++) {
        int32_t temp = sol.read(in);
        sol.forbid_[temp] = true;
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.distance_;
    return out;
}

solution& solution::operator()()
{
    if (start_ == target_) {
        distance_ = 0;
    } else {
        distance_ = bfs();
    }

    return *this;
}

int32_t solution::read(std::istream& in)
{
    int32_t a1, a2, a3, a4;
    in >> a1 >> a2 >> a3 >> a4;
    return (a1 * 1000) + (a2 * 100) + (a3 * 10) + a4;
}

int32_t solution::bfs()
{
    std::queue<int32_t> work;
    std::vector<int32_t> distances(10000, -1);
    distances[start_] = 0;
    work.push(start_);

    while (!work.empty()) {
        int32_t u(work.front());
        int32_t p(u);
        work.pop();

        for (int32_t i = 0; i < 4; i++) {
            int32_t d = static_cast<int32_t>(std::pow(10.0, static_cast<double>(i)));
            int32_t v = (p % 10 == 0) ? u + (d * 9) : u - d;

            if (!forbid_[v] && distances[v] == -1) {
                distances[v] = distances[u] + 1;

                if (v == target_) {
                    return distances[v];
                }

                work.push(v);
            }

            v = (p % 10 == 9) ? u - (d * 9) : u + d;

            if (!forbid_[v] && distances[v] == -1) {
                distances[v] = distances[u] + 1;

                if (v == target_) {
                    return distances[v];
                }

                work.push(v);
            }

            p /= 10;
        }
    }

    return -1;
}
}

void U10067::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
