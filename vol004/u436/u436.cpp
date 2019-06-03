#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u436.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
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
    U436 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), result_(), rates_() {}

        operator bool()
        {
            return not rates_.empty();
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t case_no_;
        bool result_;
        std::vector<std::vector<double_t>> rates_;
    };

    solution_t& solution_t::operator()()
    {
        ++case_no_;

        for (size_t k = 0; k < rates_.size(); ++k)
        {
            for (size_t i = 0; i < rates_.size(); ++i)
            {
                for (size_t j = 0; j < rates_.size(); ++j)
                {
                    rates_[i][j] = std::max(rates_[i][j], rates_[i][k] * rates_[k][j]);
                }
            }
        }

        result_ = false;

        for (size_t i = 0; not result_ && i < rates_.size(); ++i)
        {
            result_ = (rates_[i][i] - 1.0) > 1e-5;
        }

        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.rates_.clear();
        size_t n(0);

        if (in >> n && n > 0)
        {
            std::vector<std::pair<std::string, size_t>> currencyNames;
            currencyNames.reserve(n);

            while (currencyNames.size() < n)
            {
                std::string name;
                in >> name;
                currencyNames.push_back(std::make_pair(name, currencyNames.size()));
            }

            std::map<std::string, size_t> currencies(currencyNames.begin(), currencyNames.end());
            sol.rates_ = std::vector<std::vector<double_t>>(n, std::vector<double_t>(n));
            in >> n;

            while (n--)
            {
                std::string from, to;
                double_t rate;
                in >> from >> rate >> to;
                sol.rates_[currencies[from]][currencies[to]] = rate;
            }
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << "Case " << sol.case_no_ << ": " << (sol.result_ ? "Yes" : "No");
        return out;
    }

}

void U436::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
