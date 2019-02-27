#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10042.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10042 instance;
    instance();
}

namespace {
    class solution {
        math::sieve_t<31623> sieve_;
        int32_t n_;
    public:
        solution() : sieve_(), n_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        bool is_smith(int32_t n) const;
        static int32_t n_summ(int64_t n)
        {
            int32_t s(0);

            for (; n != 0; n /= 10) {
                s += n % 10;
            }

            return s;
        }
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.n_;
        return out;
    }

    solution& solution::operator()()
    {
        n_++;

        for (; !is_smith(n_); n_++);

        return *this;
    }

    bool solution::is_smith(int32_t n) const
    {
        int32_t s0 = n_summ(n);
        int32_t s1(0);
        int32_t upper = int32_t(std::sqrt(n) + 1);

        for (auto it = sieve_.begin() + 1; it != sieve_.end() && *it <= uint64_t(upper); ++it) {
            if (n % *it == 0) {
                int32_t s = n_summ(*it);

                for (; n % *it == 0; n /= int32_t(*it)) {
                    s1 += s;
                }
            }
        }

        if (s1 != 0 && n != 1) {
            s1 += n_summ(n);
        }

        return s0 == s1;
    }
}

void U10042::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
