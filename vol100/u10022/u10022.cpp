#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10022.h"

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
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10022 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), m_(), n_(), path_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        int32_t m_, n_;
        int32_t path_;

        void get_position(int32_t n, int32_t& r, int32_t& c);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        in >> sol.m_ >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.path_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        if (m_ > n_)
        {
            std::swap(m_, n_);
        }

        int32_t mr, mc, nr, nc;
        get_position(m_, mr, mc);
        get_position(n_, nr, nc);
        path_ = 0;

        for ( ; mr < nr; mr++)
        {
            if (mc & 1) // mc is odd
            {
                mc++, path_++;
            }
            else if (mc + 2 == nc)
            {
                mc = nc, path_ += 2;
            }
            else if (mc + 2 < nc)
            {
                mc += 2, path_ += 2;
            }
            else
            {
                path_ += 2;
            }
        }

        path_ += std::abs(nc - mc);
        return *this;
    }

    void solution_t::get_position(int32_t n, int32_t& r, int32_t& c)
    {
        r = static_cast<int32_t>(std::sqrt(static_cast<double_t>(n)));
        auto sr = r * r;

        if (sr < n)
        {
            c = n - sr;
            r++;
        }
        else
        {
            c = n - (r - 1) * (r - 1);
        }
    }
}

void U10022::operator()() const
{
    solution_t sol;

    std::string separator;

    while (std::cin >> sol && sol)
    {
        std::cout << separator << sol() << std::endl;
        separator = "\n";
    }
}
