#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10161.h"

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
    U10161 instance;
    instance();
}

namespace  {

class solution {
    int x_;
    int y_;
public:
    solution() : x_(0), y_(0) {}

    friend std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.x_ << " " << sol.y_;
        return out;
    }

    solution& operator () (int n)
    {
        int root = ceil(sqrt(n));
        int shift = root * root - root + 1;
        x_ = y_ = root;

        if(root % 2 == 0) {
            if(n > shift) {
                y_ -= n - shift;
            } else {
                x_ -= shift - n;
            }
        } else {
            if(n > shift) {
                x_ -= n - shift;
            } else {
                y_ -= shift - n;
            }
        }
        return *this;
    }
};

}  // namespace

void U10161::operator()()
{
    int n;
    solution sol;
    while (std::cin >> n && n != 0) {
        std::cout << sol(n) << std::endl;
    }
}
