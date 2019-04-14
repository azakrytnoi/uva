#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u440.h"

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
    U440 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : DP_(N_), n_(), m_()
        {
            auto backtrace = [](size_t n, size_t k) -> bool {
                std::vector<size_t> link(N_ + 1);

                for (size_t i = 2; i < n; ++i)
                {
                    link[i] = i + 1;
                }

                link[n] = 2;
                size_t head(2), prev(n), last(0);

                for (size_t i = 1; i < n; ++i)
                {
                    for (size_t j = 1; j < k; ++j) {
                        prev = head, head = link[head];
                    }

                    last = head;
                    link[prev] = link[head];
                    head = link[head];
                }

                return last == 2;
            };

            for (size_t i = 3; i < N_; ++i) {
                size_t count(2);

                while (not backtrace(i, count)) {
                    ++count;
                }

                DP_[i] = count;
            }
        }

        operator bool()
        {
            return n_ != 0;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        const static size_t N_ = 150;
        std::vector<size_t> DP_;
        size_t n_, m_;
    };

    solution_t& solution_t::operator()()
    {
        m_ = DP_[n_];
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << sol.m_;
        return out;
    }

}

void U440::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
