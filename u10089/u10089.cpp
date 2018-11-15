#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10089.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <tuple>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10089 instance;
    instance();
}

namespace {
class solution {
public:
    solution();

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return N_ != 0;
    }
    solution& operator()();

private:
    int32_t N_;
    std::vector<std::tuple<int64_t, int64_t, int64_t>> packages_;
    bool possible_;
};

solution::solution() : N_(), packages_(), possible_(false) {}

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.N_;
    sol.possible_ = false;
    sol.packages_.clear();

    if (in && sol.N_ > 0) {
        sol.packages_.reserve(sol.N_);
        std::generate_n(std::back_inserter(sol.packages_), sol.N_, [&]() -> std::tuple<int64_t, int64_t, int64_t> {
            int64_t p1(0), p2(0), p3(0);
            in >> p1 >> p2 >> p3;
            return std::make_tuple(p1, p2, p3);
        });
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << (sol.possible_ ? "Yes" : "No");
    return out;
}

solution& solution::operator()()
{
    std::vector<std::pair<int64_t, int64_t>> repack;
    repack.reserve(packages_.size());
    std::transform(packages_.begin(), packages_.end(),
    std::back_inserter(repack), [](const std::tuple<int64_t, int64_t, int64_t>& pack) -> std::pair<int64_t, int64_t> {
        return std::make_pair(std::get<0>(pack) - std::get<2>(pack), std::get<1>(pack) - std::get<2>(pack));
    });
    int64_t x1(repack[0].first), y1(repack[0].second);
    int64_t x2(x1), y2(y1);

    for (auto it = repack.begin() + 1; it != repack.end(); ++it) {
        int64_t x = it->first, y = it->second;
        int64_t a1 = y1 * x, b1 = x1 * y, a2 = y2 * x, b2 = x2 * y;

        if (a1 == b1) {
            if (x1 * x + y1 * y <= 0) {
                possible_ = true;
                break;

            } else {
                continue;
            }
        }

        if (a2 == b2) {
            if (x2 * x + y2 * y <= 0) {
                possible_ = true;
                break;

            } else {
                continue;
            }
        }

        if (a1 < b1) {
            x1 = x, y1 = y;
        }

        if (a2 > b2) {
            x2 = x, y2 = y;
        }

        if (x1 == x2 && y1 == y2) {
            possible_ = true;
            break;
        }

        if (y1 * x2 == y2 * x1) {
            if (x1 * x2 + y1 * y2 <= 0) {
                possible_ = true;
                break;

            } else {
                continue;
            }
        }

        if (y1 * x2 < y2 * x1) {
            possible_ = true;
            break;
        }
    }

    return *this;
}
}

void U10089::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
