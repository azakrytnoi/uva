#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10783.h"

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
    U10783 instance;
    instance();
}

namespace {

    class solution {
        int32_t a_, b_;
        int64_t summ_;
    public:
        solution() : a_(), b_(), summ_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
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
        in >> sol.a_ >> sol.b_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.summ_;
        return out;
    }

    solution& solution::operator()()
    {
        summ_ = 0;

        for (int32_t i = a_; i <= b_; i++) {
            if (i & 0x01) {
                summ_ += i;
            }
        }

        return *this;
    }

}

void U10783::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;
    int32_t caseNo(0);

    while (N--) {
        std::cin >> sol;
        std::cout << "Case " << (++caseNo) << ": " << sol() << std::endl;
    }
}
