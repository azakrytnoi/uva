#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10040.h"

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
    U10040 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), n_(), k_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        size_t n_, k_;
        uint64_t result_;

        void dfs(std::vector<std::vector<char>>& gen, std::vector<char>::iterator& ptr, int64_t node, const uint64_t mask);
        int64_t min_exp(const std::vector<char>& vec, const int64_t len);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        in >> sol.n_ >> sol.k_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.result_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<std::vector<char>> order (2, std::vector<char>(1 << n_));
        std::vector<std::vector<char>> gen(1 << n_, std::vector<char>(2));
        uint64_t mask ((1 << (n_ - 1)) - 1);

        for (auto i  = (1 << (n_ - 1)) - 1; i >= 0; --i)
        {
            gen[i][0]++, gen[i][1]++;
        }

        auto ptr(order[0].begin());
        dfs(gen, ptr, 0, mask);
        auto len (std::distance(order[0].begin(), ptr));

        for (auto i = len - 1, j = 0l; i >= 0; --i, ++j)
        {
            order[1][i] = order[0][j];
        }

        auto o1(min_exp(order[0], len));
        auto o2(min_exp(order[1], len));
        auto flip(false);

        for (auto i = 0; i < len; ++i)
        {
            if (order[0][(o1 + 1) % len] != order[1][(o2 + i) % len])
            {
                flip = order[0][(o1 + 1) % len] > order[1][(o2 + i) % len];
                break;
            }
        }

        result_ = 0;

        for (auto i = 0; i < len; ++i)
        {
            result_ <<= 1;

            if (flip)
            {
                result_ |= order[0][(o1 + i + k_) % len] - '0';
            }
            else
            {
                result_ |= order[1][(o2 + i + k_) % len] - '0';
            }
        }

        result_ -= (n_ + 1);
        result_ /= (n_ + 1);

        return *this;
    }

    void solution_t::dfs(std::vector<std::vector<char>>& gen, std::vector<char>::iterator& ptr, int64_t node, const uint64_t mask)
    {
        auto prefix = node;
        prefix <<= 1;

        for (auto i = 0; i < 2; i++)
        {
            if (gen[node][i])
            {
                gen[node][i]--;
                dfs(gen, ptr, (prefix | i) & mask, mask);
                *ptr = i + '0', ++ptr;
            }
        }
    }

    int64_t solution_t::min_exp(const std::vector<char>& vec, const int64_t len)
    {
        int64_t i(0), j(1), k(0);

        while (i < len && j < len && k < len)
        {
            auto x((i + k)%len);
            auto y((j + k)%len);

            if (vec[x] == vec[y])
            {
                ++k;
            }
            else if (vec[x] > vec[y])
            {
                i = j + 1 > i + k + 1 ? j + 1 : i + k + 1;
                k = 0;
                std::swap(i, j);
            }
            else
            {
                j = i + 1 > j + k + 1 ? i + 1 : j + k + 1;
                k = 0;
            }
        }

        return i;
    }

}

void U10040::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
