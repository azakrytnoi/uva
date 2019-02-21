#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u400.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <limits>
#include <iomanip>
#include <ios>

U400::U400() {}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U400 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_(), names_(), nrows_(), col_width_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t n_;
        std::vector<std::string> names_;
        size_t nrows_;
        size_t col_width_;
    };

    solution_t& solution_t::operator()()
    {
        std::sort(names_.begin(), names_.end());
        col_width_ = std::accumulate(names_.begin(), names_.end(), 0, [](const size_t prev, const std::string & str) {
            return prev < str.length() ? str.length() : prev;
        });
        col_width_ += 2;

        if (col_width_ > 60) {
            col_width_ = 60;
        }

        auto ncols = col_width_ == 31 ? 2 : 60 / col_width_;
        nrows_ = n_ / ncols + (n_ % ncols != 0 ? 1 : 0);
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.names_.clear();

        if (in >> sol.n_) {
            sol.names_.reserve(sol.n_);
            std::istream_iterator<std::string> sin(in);
            std::copy_n(sin, sol.n_, std::back_inserter(sol.names_));
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << std::string(60, '-') << std::endl;

        for (size_t i = 0; i < sol.nrows_; ++i) {
            for (size_t j = 0; i + j < sol.n_; j += sol.nrows_) {
                if (j > 0) {
                    out << "  ";
                }

                out << std::left << std::setw(sol.col_width_ - 2) << sol.names_[i + j];
            }

            out << std::endl;
        }

        return out;
    }


}  // namespace

void U400::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
