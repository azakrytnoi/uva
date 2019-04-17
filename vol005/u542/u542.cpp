#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u542.h"

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
    U542 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : names_(), chances_(16, std::vector<double_t>(16)) {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()()
        {
            std::vector<double_t> probability(16);

            for (size_t round = 0, group_size = 1; round < 4; ++round, group_size *= 2)
            {
                for (size_t i = 0; i < 16; ++i)
                {
                    size_t current_group = i / group_size;
                    size_t start = current_group % 2 == 0 ? current_group * group_size + group_size : current_group * group_size - group_size;
                    probability[i] = 0;

                    for (size_t shift = 0; shift < group_size; ++shift)
                    {
                        probability[i] += names_[start + shift].second * chances_[i][start + shift];
                    }

                    probability[i] *= names_[i].second;
                }

                std::transform(names_.begin(), names_.end(), probability.begin(), names_.begin(), [](std::pair<std::string, double_t>& lhs, double_t rhs)
                {
                    lhs.second = rhs;
                    return lhs;
                });
            }

            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            if (in)
            {
                std::generate_n(std::back_inserter(sol.names_), 16, [&]()
                {
                    std::string name;
                    in >> name;
                    return std::make_pair(name, 1.0);
                });

                if (in)
                {
                    std::for_each(sol.chances_.begin(), sol.chances_.end(), [&](std::vector<double_t>& row)
                    {
                        std::generate(row.begin(), row.end(), [&]()
                        {
                            double_t chance;
                            in >> chance;
                            return chance / 100.0;
                        });
                    });
                }
            }

            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            std::for_each(sol.names_.begin(), sol.names_.end(), [&](const std::pair<std::string, double_t>& name)
            {
                out << std::left << std::setw(10) << name.first << std::setw(1) << " p=" << std::fixed << //
                    std::setprecision(2) << name.second * 100.0 << '%' << std::endl;
            });
            return out;
        }

    private:
        std::vector<std::pair<std::string, double_t>> names_;
        std::vector<std::vector<double_t>> chances_;
    };

}

void U542::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
