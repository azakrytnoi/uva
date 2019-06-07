#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u696.h"

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
    U696 instance;
    instance();
}

namespace {

    class solution {
        uint32_t knights_;
        uint32_t n_;
        uint32_t m_;

    public:
        solution() :
            knights_(0), n_(0), m_(0)
        {}

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            in >> sol.n_ >> sol.m_;
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol)
        {
            out << sol.knights_ << " knights may be placed on a " << sol.n_ << " row " << sol.m_ << " column board.";
            return out;
        }

        operator bool() const
        {
            return n_ + m_ != 0;
        }

        solution& operator ()()
        {
            uint32_t n(n_);
            uint32_t m(m_);

            if (n > m)
            {
                std::swap(n, m);
            }

            if (n == 1)
            {
                knights_ = m;

            }
            else if (n == 2)
            {
                knights_ = m / 4 * 4 + (m % 4 == 1 ? 2 : (m % 4 >= 2 ? 4 : 0));
            }
            else
            {
                knights_ = (n * m + 1) / 2;
            }

            return *this;
        }
    };

}  // namespace

void U696::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
