#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u623.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include "bigint.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U623 instance;
    instance();
}

namespace  {

class solution {
    math::BigInteger int_;
    uint32_t n_;
public:
    solution() : int_(), n_(0) {}

    friend std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_;
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.n_ << "!" << std::endl << sol.int_;
        return out;
    }

    solution& operator()()
    {
        int_ = std::string("1");
        for (uint32_t n = 2; n <= n_; n++) {
            int_ *= n;
        }
        return *this;
    }
};

}  // namespace

void U623::operator()()
{
    solution sol;
    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
