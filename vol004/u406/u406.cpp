#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u406.h"

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
    U406 instance;
    instance();
}

namespace {

    class solution {
        math::sieve_t<1000> sieve_;
        uint32_t N_, C_;
        std::pair<int32_t, int32_t> result_;
    public:
        solution() : sieve_(), N_(0), C_(0), result_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            in >> sol.N_ >> sol.C_;
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        solution& operator ()();

    private:
    };

    solution& solution::operator ()()
    {
        int64_t bound = std::lower_bound(sieve_.begin(), sieve_.end(), N_) - sieve_.begin();

        while (bound >= sieve_.size() || sieve_[bound] > N_) {
            --bound;
        }

        ++bound;
        int64_t size = (bound % 2) ? (C_ * 2 - 1) : (C_ * 2);
        result_.first = (bound - size) > 0 ? (bound - size) / 2 : 0;
        result_.second = (bound - size) > 0 ? size : bound;
        return *this;
    }

    std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        out << sol.N_ << " " << sol.C_ << ": ";
        std::ostream_iterator<uint32_t> oit (out, " ");
        std::copy_n(sol.sieve_.begin() + sol.result_.first, sol.result_.second, oit);
        out << std::endl;
        return out;
    }

}

void U406::operator()() const
{
    solution sol;

    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
