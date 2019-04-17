#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u12293.h"

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
    U12293 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_(), win_() {}

        operator bool()
        {
            return n_ != 0;
        }

        solution_t& operator()()
        {
            size_t curr(1);

            while (curr < n_)
            {
                curr = curr * 2 + 1;
            }

            win_ = curr == n_;
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
            out << (sol.win_ ? "Bob" : "Alice");
            return out;
        }

    private:
        size_t n_;
        bool win_;
    };

}

void U12293::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
