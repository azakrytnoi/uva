#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10152.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10152 instance;
    instance();
}

namespace {
class solution {
public:
    solution() : original_(), target_(), shifts_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool();

private:
    std::vector<std::string> original_;
    std::map<std::string, size_t> target_;
    std::vector<std::pair<std::string, size_t>> shifts_;
};

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    std::for_each(sol.shifts_.begin(), sol.shifts_.end(), [&](const std::pair<std::string, size_t>& arg) {
        out << arg.first << std::endl;
    });
    return out;
}

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.original_.clear();
    sol.target_.clear();
    size_t nTurtles;
    in >> nTurtles;

    if (in) {
        sol.original_.reserve(nTurtles);
        std::string name;
        std::getline(in, name);
        std::generate_n(std::back_inserter(sol.original_), nTurtles, [&]() {
            std::getline(in, name);
            return name;
        });

        for (size_t j = 0; j < nTurtles; j++) {
            std::getline(in, name);
            sol.target_[name] = j;
        }
    }

    return in;
}

solution::operator bool()
{
    shifts_.clear();
    std::vector<std::pair<std::string, size_t>> order;
    order.reserve(original_.size());
    std::transform(original_.begin(), original_.end(), std::back_inserter(order), [&](const std::string & name) {
        return std::make_pair(name, target_[name]);
    });
    shifts_.reserve(order.size());

    for (size_t i = original_.size() - 1, next = original_.size() - 1; ; i--) {
        if (order[i].second != next) {
            shifts_.push_back(order[i]);

        } else {
            next--;
        }

        if (i == 0) {
            break;
        }
    }

    std::sort(shifts_.begin(), shifts_.end(), [](std::pair<std::string, size_t>& l, std::pair<std::string, size_t>& r) {
        return l.second > r.second;
    });
    return true;
}
}

void U10152::operator()() const
{
    int N;
    std::cin >> N;
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol << std::endl;
    }
}
