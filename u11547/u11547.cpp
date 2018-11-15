#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11547.h"

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
    U11547 instance;
    instance();
}

namespace {

class solution {
    int64_t n_;
public:
    solution() : n_(0) {}

    friend std::istream& operator >>(std::istream& in, solution& sol)
    {
        in >> sol.n_;
        return in;
    }

    friend std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        out << sol.n_;
        return out;
    }

    solution& operator ()()
    {
        n_ *= 567;
        n_ /= 9;
        n_ += 7492;
        n_ *= 235;
        n_ /= 47;
        n_ -= 498;
        n_ /= 10;
        n_ = std::abs(n_ % 10);
        return *this;
    }

private:
};

}

void U11547::operator()() const
{
    int32_t n_cases;
    std::cin >> n_cases;
    solution sol;

    while (n_cases--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
