#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10229.h"

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
    U10229 instance;
    instance();
}

namespace {

    template<size_t R, size_t C, typename T>
    class matrix_t {
    public:
        matrix_t() : data_(std::vector<std::vector<T>>(R, std::vector<T>(C))) {}

        std::vector<T>& operator[](size_t row)
        {
            return data_[row];
        }

        matrix_t multiply (const matrix_t& rhs) const
        {
            matrix_t res;

            for (size_t i = 0; i < R; ++i)
            {
                for (size_t j = 0; j < C; ++j)
                {
                    for (size_t k = 0; k < C; ++k)
                    {
                        res[i][j] += *this[i][k] * rhs[k][j];
                    }
                }
            }

            return res;
        }

        matrix_t pow(uint16_t n) const
        {
            if (n == 0)
            {
                matrix_t res;

                for (size_t i = 0; i < R && i < C; ++i)
                {
                    res[i][i] = 1;
                }

                return res;
            }
            else if (n % 2 == 0)
            {
                matrix_t res = pow(n / 2);
                return res.multiply(res);
            }
            else
            {
                return multiply(pow(n - 1));
            }
        }

        matrix_t multiplyMod(const matrix_t other, uint32_t m) const
        {
            matrix_t res;

            for (size_t i = 0; i < R; ++i)
            {
                for (size_t j = 0; j < C; ++j)
                {
                    for (size_t k = 0; k < C; ++k)
                    {
                        res[i][j] = (res[i][j] + data_[i][k] * other.data_[k][j]) % m;
                    }
                }
            }

            return res;
        }

        matrix_t powMod(uint32_t n, uint32_t m) const
        {
            if (n == 0)
            {
                matrix_t res;

                for (size_t i = 0; i < R && i < C; ++i)
                {
                    res[i][i] = 1;
                }

                return res;
            }
            else if (n % 2 == 0)
            {
                matrix_t res = powMod(n / 2, m);
                return res.multiplyMod(res, m);
            }
            else
            {
                return multiplyMod(powMod(n - 1, m), m);
            }
        }

    private:
        std::vector<std::vector<T>> data_;
    };

    class solution_t {
    public:
        solution_t() : n_(), m_(), fib_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        uint32_t n_, m_;
        uint64_t fib_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.n_ >> sol.m_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.fib_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        uint32_t m(1);
        m <<= m_;
        matrix_t<2, 2, uint32_t> fib;
        fib[0][0] = fib[0][1] = fib[1][0] = 1;
        fib[1][1] = 0;
        fib = fib.powMod(n_, m);
        fib_ = fib[1][0];
        return *this;
    }

}

void U10229::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
