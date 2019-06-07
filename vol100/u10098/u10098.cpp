#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10098.h"

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
    U10098 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), word_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::string word_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        in >> sol.word_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        auto cmp = [](char lhs, char rhs) -> bool
        {
            if (lhs >= 'A')
            {
                return (rhs < 'A' || lhs < rhs);
            }
            else
            {
                return (rhs < 'A' && lhs < rhs);
            }
        };
        std::string word(sol.word_.begin(), sol.word_.end());
        std::sort(word.begin(), word.end(), cmp);

        do
        {
            out << word << std::endl;
        }
        while (std::next_permutation(word.begin(), word.end(), cmp));

        return out;
    }

    solution_t& solution_t::operator()()
    {
        return *this;
    }

}

void U10098::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
