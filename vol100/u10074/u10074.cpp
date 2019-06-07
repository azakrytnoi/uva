#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10074.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10074 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : map_(), max_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not map_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<std::vector<bool>> map_;
        size_t max_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.map_.clear();
        size_t n(0), m(0);

        if (in >> n >> m && n > 0 && m > 0)
        {
            sol.map_ = std::vector(n, std::vector<bool>(m));

            for (auto i = 0u; i < n; ++i)
            {
                for (auto j = 0u; j < m; ++j)
                {
                    uint16_t tmp(0);
                    in >> tmp;
                    sol.map_[i][j] = tmp == 1;
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.max_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        max_ = 0;
        size_t length(0), width(0);

        for (auto i = 0u; i < map_.size(); ++i)
        {
            std::vector<size_t> part_summ(map_[i].size());

            for (auto j = i; j < map_.size(); ++j)
            {
                size_t tmp(0);
                for (auto k = 0u; k < map_[i].size(); ++k)
                {
                    part_summ[k] += map_[j][k] ? 0 : 1;

                    if (tmp != length * width)
                    {
                        tmp = 0, length = 0;
                    }

                    ++length, width = j - i + 1;
                    tmp += part_summ[k];

                    if (tmp == length * width)
                    {
                        max_ = std::max(max_, tmp);
                    }
                }
            }
        }

        return *this;
    }

}

void U10074::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
