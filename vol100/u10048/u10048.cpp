#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10048.h"

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
    U10048 instance;
    instance();
}

namespace {

    struct point_t : public std::pair<int32_t, int32_t>
    {
        point_t (int32_t a = 0, int32_t b = 0) : std::pair<int32_t, int32_t>(a, b) {}
        friend std::istream& operator >> (std::istream& in, std::pair<int32_t, int32_t>& p)
        {
            in >> p.first >> p.second;
            return in;
        }
    };

    class solution_t {
    public:
        solution_t() : case_(0), req_(), res_(), matrix_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not req_.empty();
        }
        solution_t& operator()();

    private:
        size_t case_;
        std::vector<point_t> req_;
        std::vector<std::string> res_;
        std::vector<std::vector<int32_t>> matrix_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        int32_t n, m, q;
        sol.req_.clear();

        if (in >> n >> m >> q && n != 0 && m != 0 && q != 0)
        {
            sol.matrix_ = std::vector(n + 1,
                                      std::vector<int32_t>(n + 1, std::numeric_limits<int32_t>::max()));

            for (auto i = 1; i <= n; ++i)
            {
                sol.matrix_[i][i] = 0;
            }

            while (m--)
            {
                int32_t i, j, b;
                in >> i >> j >> b;
                sol.matrix_[j][i] = sol.matrix_[i][j] = std::min(sol.matrix_[i][j], b);
            }

            for (auto k = 1; k <= n; ++k)
            {
                for (auto i = 1; i <= n; ++i)
                {
                    for (auto j = 1; j <= n; ++j)
                    {
                        sol.matrix_[i][j] = std::min(sol.matrix_[i][j],
                                                     std::max(sol.matrix_[i][k], sol.matrix_[k][j]));
                    }
                }
            }

            sol.req_.reserve(q);
            std::istream_iterator<point_t> pin(in);
            std::copy_n(pin, q, std::back_inserter(sol.req_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Case #" << sol.case_ << std::endl;
        std::ostream_iterator<std::string> sout(out, "\n");
        std::copy(sol.res_.begin(), sol.res_.end(), sout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        ++case_;
        res_.clear();
        res_.reserve(req_.size());
        std::transform(req_.begin(), req_.end(), std::back_inserter(res_), [&](auto q) -> std::string
        {
            if (matrix_[q.first][q.second] != std::numeric_limits<int32_t>::max())
            {
                std::stringstream temp;
                temp << matrix_[q.first][q.second];
                return temp.str();
            }

            return "no path";
        });
        return *this;
    }

}

void U10048::operator()() const
{
    solution_t sol;
    bool next(false);

    while (std::cin >> sol && sol)
    {
        if (next)
        {
            std::cout << std::endl;
        }

        std::cout << sol() << std::endl;
    }
}
