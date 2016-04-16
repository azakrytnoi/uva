#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10038.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <bitset>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10038 instance;
    instance();
}

namespace
{

class solution
{
    bool jolly_;
    std::vector<bool> set_;
public:
    solution() : jolly_(false), set_()
    {
        set_.reserve(3000);
    }

    friend std::istream& operator >>(std::istream& in, solution& sol);

    friend std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        out << (sol.jolly_ ? "Jolly" : "Not jolly");
        return out;
    }

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    size_t n;
    sol.jolly_ = true;
    if (in >> n && n > 0) {
        size_t n1(n - 1);
        sol.set_.clear();
        sol.set_.resize(n1);
        int last;
        in >> last;
        n--;
        if (n > 0) {
            while (n--) {
                int next;
                in >> next;
                size_t diff = std::abs(last - next);
                if (diff <= n1 && diff > 0) {
                    sol.set_[diff - 1] = true;
                } else {
                    sol.jolly_ = false;
                }
                last = next;
            }
            sol.jolly_ = sol.jolly_ && std::accumulate(sol.set_.begin(), sol.set_.end(), true, [](auto i, auto next) {
                return i && next;
            });
        }
    }
    return in;
}

}

void U10038::operator()() const
{
    solution sol;
    while (std::cin >> sol) {
        std::cout << sol << std::endl;
    }
}
