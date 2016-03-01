#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u900.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace {
template <size_t MAX>
class fibonacci {
    std::vector<uint64_t> fib_;

public:
    fibonacci() : fib_()
    {
        fib_.reserve(MAX + 1);
        uint64_t n0[] = {1, 1};
        fib_.assign(n0, n0 + 2);
    }

    uint64_t operator ()(int n)
    {
        while (fib_.size() < size_t(n + 1)) {
            size_t last = fib_.size() - 1;
            fib_.push_back(fib_[last] + fib_[last - 1]);
        }
        return fib_[n];
    }
};
}

U900::U900() {}

extern "C" { 	UVA_API_EXPORT void __cdecl invoke(); } void __cdecl invoke() { 	U900 instance; 	instance(); }
void U900::operator()()
{
    int N;
    fibonacci<50> fib;
    while (std::cin >> N && N > 0) {
        std::cout << fib (N) << std::endl;
    }
}
