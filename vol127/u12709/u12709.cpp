#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u12709.h"

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
    U12709 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : max_(0), ants_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !ants_.empty();
        }

        solution& operator ()();

    private:
        uint64_t max_;
        std::vector<std::pair<uint16_t, uint64_t>> ants_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.ants_.clear();
        int n;

        if (in >> n && n > 0)
        {
            sol.ants_.reserve(n);
            std::generate_n(std::back_inserter(sol.ants_), n, [&]() -> std::pair<uint16_t, uint64_t>
            {
                uint16_t l(0), w(0), h(0);
                in >> l >> w >> h;
                return std::make_pair(h, l* w * h);
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.max_;
        return out;
    }

    solution& solution::operator ()()
    {
        std::sort(ants_.begin(), ants_.end(), [](auto a1, auto a2) -> bool
        {
            if (a1.first == a2.first) return a1.second > a2.second;
            return a1.first > a2.first;
        });
        max_ = ants_.front().second;
        return *this;
    }

}

void U12709::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
