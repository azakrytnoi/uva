#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10252.h"

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
    U10252 instance;
    instance();
}

namespace
{

class solution
{
    std::string a_;
    std::string b_;
    std::string result_;

public:
    solution() : a_(), b_(), result_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::getline(in, sol.a_);
        std::getline(in, sol.b_);
        sol.result_.clear();
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.result_;
        return out;
    }

    solution& operator () ()
    {
        std::sort(a_.begin(), a_.end());
        std::sort(b_.begin(), b_.end());

        auto pa = a_.begin();
        auto pb = b_.begin();
        while (pa != a_.end() && pb != b_.end()) {
            if (*pa == *pb) {
                result_ += (*pa++);
                ++pb;
            } else if (*pa > *pb) {
                ++pb;
            } else {
                ++pa;
            }
        }
        return *this;
    }
};

}  // namespace

void U10252::operator()() const
{
    solution sol;
    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
