#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11984.h"

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
    U11984 instance;
    instance();
}

namespace {

    class solution {
        double c_, f_;
        double result_;
    public:
        solution() : c_(), f_(), result_() {}

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
        in >> sol.c_ >> sol.f_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << std::fixed << std::setprecision(2) << sol.result_;
        return out;
    }

    solution& solution::operator()()
    {
        result_ = (((c_ * 9) / 5) + 32 );
        result_ += f_;
        result_ = ((result_ - 32) * 5 / 9);
        return *this;
    }

}

void U11984::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;
    int32_t caseNo(0);

    while (N--)
    {
        std::cin >> sol;
        std::cout << "Case " << (++caseNo) << ": " << sol() << std::endl;
    }
}
