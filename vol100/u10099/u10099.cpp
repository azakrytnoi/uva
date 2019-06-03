#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10099.h"

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
    U10099 instance;
    instance();
}

namespace {

    constexpr int32_t INF = std::numeric_limits<int32_t>::min();

    class solution {
        int32_t N_, roads_, source_, destination_, tourists_;
        std::vector<std::vector<int32_t>> matrix_;
        int32_t trip_;
    public:
        solution() : N_(), roads_(), source_(), destination_(), tourists_(), matrix_(), trip_()
        {
            matrix_.reserve(100);
        }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !(N_ == 0 && roads_ == 0);
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.N_ >> sol.roads_;

        if (sol)
        {
            sol.matrix_.clear();
            std::generate_n(std::back_inserter(sol.matrix_), sol.N_ + 1, [&]()
            {
                return std::vector<int32_t>(sol.N_ + 1, INF);
            });

            for (int32_t i = 0; i < sol.roads_; i++)
            {
                int32_t c1, c2, p;
                in >> c1 >> c2 >> p;
                sol.matrix_[c1 - 1][c2 - 1] = sol.matrix_[c2 - 1][c1 - 1] = p - 1;
            }

            for (int32_t i = 0; i < sol.N_; i++)
            {
                sol.matrix_[i][i] = 0;
            }

            in >> sol.source_ >> sol.destination_ >> sol.tourists_;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.trip_;
        return out;
    }

    solution& solution::operator()()
    {
        if (source_ == destination_)
        {
            trip_ = 1;
        }
        else
        {
            for (int32_t k = 0; k < N_; k++)
            {
                for (int32_t i = 0; i < N_; i++)
                {
                    for (int32_t j = 0; j < N_; j++)
                    {
                        matrix_[i][j] = std::max(matrix_[i][j], std::min(matrix_[i][k], matrix_[k][j]));
                    }
                }
            }

            trip_ = (tourists_ + matrix_[source_ - 1][destination_ - 1] - 2) / (matrix_[source_ - 1][destination_ - 1] - 1);
        }

        return *this;
    }

}

void U10099::operator()() const
{
    solution sol;
    int32_t caseNo(0);

    while (std::cin >> sol && sol)
    {
        std::cout << "Scenario #" << (++caseNo) << std::endl << "Minimum Number of Trips = " << sol() << std::endl << std::endl;;
    }
}
