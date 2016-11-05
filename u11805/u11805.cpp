#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11805.h"

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
    U11805 instance;
    instance();
}

namespace {
    class solution {
        int32_t N_, K_, P_;
        int32_t pos_;
        size_t caseNo_;
    public:
        solution() : N_(), K_(), P_(), pos_(), caseNo_(0) {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.N_ >> sol.K_ >> sol.P_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << "Case " << sol.caseNo_ << ": " << sol.pos_;
        return out;
    }

    solution& solution::operator()()
    {
        ++caseNo_;
        pos_ = K_ + P_;

        while (pos_ > N_) {
            pos_ -= N_;
        }

        return *this;
    }
}

void U11805::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
