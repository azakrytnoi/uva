#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u12469.h"

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
    U12469 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : fib_(1001), n_()
        {
            size_t a0(2), a1(3), a2(5);
            fib_[a0] = fib_[a1] = true;

            while (a2 < 1000)
            {
                fib_[a2] = true;
                a0 = a2;
                a2 += a1;
                a1 = a0;
            }
        }

        operator bool()
        {
            return n_ != 0;
        }

        solution_t& operator()()
        {
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            sol.n_ = 0;
            in >> sol.n_;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << (sol.fib_[sol.n_] ? "Roberto" : "Alicia");
            return out;
        }

    private:
        std::vector<bool> fib_;
        size_t n_;
    };

}

void U12469::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
