#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u435.h"

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
    U435 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(std::numeric_limits<size_t>::max()), parties_(), indexes_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return case_no_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t case_no_;
        std::vector<uint16_t> parties_;
        std::vector<uint32_t> indexes_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.parties_.clear();
        sol.indexes_.clear();

        if (sol.case_no_ == std::numeric_limits<size_t>::max()) {
            in >> sol.case_no_;
        }

        size_t n(0);

        if (in >> n) {
            sol.parties_.reserve(n);
            sol.indexes_.resize(n);
            std::istream_iterator<uint16_t> iin(in);
            std::copy_n(iin, n, std::back_inserter(sol.parties_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        for (size_t idx = 0; idx < sol.indexes_.size(); ++idx) {
            out << "party " << (idx + 1) << " has power index " << sol.indexes_[idx] << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto summ = std::accumulate(parties_.begin(), parties_.end(), 0);
        auto half = summ / 2 + (summ & 1);

        for (size_t i = 0; i < parties_.size(); ++i) {
            std::vector<int32_t> dp (1001);
            dp[0] = 1;

            for (size_t j = 0; j < parties_.size(); ++j) {
                if (i == j)	{
                    continue;
                }

                for (auto k = summ - parties_[j]; k >= 0; --k) {
                    dp[k + parties_[j]] += dp[k];
                }
            }

            int32_t index (0);

            for (auto j = half - parties_[i]; j < half && j >= 0; j++) {
                index += dp[j];
            }

            indexes_[i] = index;
        }

        return *this;
    }

}

void U435::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
