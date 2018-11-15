#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10034.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10034 instance;
    instance();
}

namespace {

class solution {
    std::vector<std::pair<double, double>> points_;
    double result_;
public:
    solution() : points_(), result_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return true;
    }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    size_t n;
    in >> n;
    sol.points_.clear();
    sol.points_.reserve(n);
    std::generate_n(std::back_inserter(sol.points_), n, [&]() {
        double x, y;
        in >> x >> y;
        return std::make_pair(x, y);
    });
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << std::fixed << std::setprecision(2) << sol.result_;
    return out;
}

solution& solution::operator()()
{
    std::vector<bool> intree(points_.size());
    intree[0] = true;
    result_ = 0.0;
    size_t cnt(1);

    while (cnt < points_.size()) {
        double min = std::numeric_limits<double>::min();
        int64_t minp(-1);

        for (size_t i = 0; i < points_.size(); i++) {
            if (intree[i]) {
                for (size_t j = 0; j < points_.size(); j++) {
                    if (!intree[j]) {
                        double dx(points_[i].first - points_[j].first);
                        double dy(points_[i].second - points_[j].second);
                        double distance(std::sqrt(dx * dx + dy * dy));

                        if (min == std::numeric_limits<double>::min() || distance < min) {
                            min = distance;
                            minp = j;
                        }
                    }
                }
            }
        }

        if (minp > 0) {
            intree[minp] = true;
            result_ += min;
        }

        cnt++;
    }

    return *this;
}

}

void U10034::operator()() const
{
    size_t N;
    std::cin >> N;
    solution sol;
    bool first(true);

    while (N--) {
        if (!first) {
            std::cout << std::endl;
        }

        first = false;
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
