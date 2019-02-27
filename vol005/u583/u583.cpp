#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u583.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U583 instance;
    instance();
}

namespace {

    class solution {
        math::sieve_t<50000> sieve_;
        std::vector<uint32_t> factors_;
        int32_t n_;
    public:
        solution () : sieve_(), factors_(), n_(0) {}

        friend std::istream& operator >> (std::istream& in, solution& sol)
        {
            in >> sol.n_;
            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const solution& sol);

        operator bool () const
        {
            return n_ != 0;
        }

        solution& operator () ();

    };

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.n_ << (sol.n_ < 0 ? " = -1 x " : " = ") << sol.factors_[0];
        std::for_each(sol.factors_.begin() + 1, sol.factors_.end(), [&](auto factor) {
            out << " x " << factor;
        });
        return out;
    }

    solution& solution::operator ()()
    {
        uint32_t num = std::abs(n_);
        factors_.clear();
        auto factor = sieve_.begin() + 1;

        while ((*factor) * (*factor) <= num) {
            while (num % (*factor) == 0) {
                num /= *factor;
                factors_.push_back(*factor);
            }

            ++factor;
        }

        if (num != 1) {
            factors_.push_back(num);
        }

        return *this;
    }

}  // namespace

void U583::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
