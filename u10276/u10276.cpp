#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10276.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10276 instance;
    instance();
}

namespace {

class solution {
    size_t n_;
    size_t result_;
public:
    solution() : n_(), result_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return true;
    }
    solution& operator()();

private:
    void simulate(size_t v, size_t p, std::vector<size_t>& pegs);
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.n_;
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.result_;
    return out;
}

solution& solution::operator()()
{
    static std::vector<size_t> pegs;
    pegs.clear();
    pegs.resize(n_ + 1);
    result_ = 0;
    simulate(1, 0, pegs);
    return *this;
}

void solution::simulate(size_t ball, size_t peg, std::vector<size_t>& pegs)
{
    if (peg != n_) {
        if (pegs[peg] == 0) {
            pegs[peg] = ball;
            result_++;
            simulate(ball + 1, peg, pegs);
            return;
        }

        for (size_t i = 0; i <= peg; i++) {
            size_t root = static_cast<size_t>(std::sqrt(pegs[i] + ball));

            if (root * root == pegs[i] + ball) {
                pegs[i] = ball;
                result_++;
                simulate(ball + 1, peg, pegs);
                return;
            }
        }

        simulate(ball, peg + 1, pegs);
    }
}

}

void U10276::operator()() const
{
    size_t N;
    std::cin >> N;
    solution sol;

    while (N-- > 0) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
