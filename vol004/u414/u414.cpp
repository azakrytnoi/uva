#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u414.h"

#include <iostream>
#include <iomanip>
#include <vector>
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
    U414 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_(), surface_(), sum_() {}

        operator bool()
        {
            return n_ > 0;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t n_;
        std::vector<std::string> surface_;
        int64_t sum_;
    };

    solution_t& solution_t::operator()()
    {
        int64_t shift(std::numeric_limits<int64_t>::max());
        std::vector<int64_t> pattern(n_);
        auto pat(pattern.begin());

        for (auto& line : surface_) {
            auto ptr(line.begin());

            for (; ptr != line.end() && *ptr == 'X'; ++ptr);

            auto left(std::distance(line.begin(), ptr));

            for (; ptr != line.end() && *ptr != 'X'; ++ptr);

            auto right(std::distance(line.begin(), ptr));
            *pat = right - left;

            if (*pat < shift) {
                shift = *pat;
            }

            ++pat;
        }

        sum_ = std::accumulate(pattern.begin(), pattern.end(), 0, [&](const int64_t old, const int64_t val) {
            return old + val - shift;
        });
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.surface_.clear();

        if (in >> sol.n_) {
            in.ignore();
            sol.surface_.reserve(sol.n_);
            std::string line;
            std::generate_n(std::back_inserter(sol.surface_), sol.n_, [&]() {
                std::getline(in, line);
                return line;
            });
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << sol.sum_;
        return out;
    }

}

void U414::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
