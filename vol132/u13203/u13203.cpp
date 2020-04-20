#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u13203.h"

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
    U13203 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return n_ > 0;
        }
        solution_t& operator()();

    private:
        uint32_t n_{0};
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.n_ = 0;
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << (sol.n_ - 2) * (sol.n_ - 1) / 2;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        return *this;
    }

}

void U13203::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
