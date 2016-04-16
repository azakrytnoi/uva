#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10189.h"

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
    U10189 instance;
    instance();
}

namespace
{

class solution
{
public:
    solution() : field_(), N_(0), M_(0) {}

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return N_ + M_ != 0;
    }

    solution& operator ()();

private:
    std::vector<std::string> field_;
    int N_, M_;

    void calculate (int n, int m);
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.N_ >> sol.M_;
    if (sol) {
        sol.field_.clear();
        sol.field_.reserve(sol.N_);
        std::string line;
        std::getline(in, line);
        std::generate_n(std::back_inserter(sol.field_), sol.N_, [&]() -> std::string {
            std::getline(in, line);
            return line;
        });
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    std::ostream_iterator<std::string> oit(out, "\n");
    std::copy(sol.field_.begin(), sol.field_.end(), oit);
    return out;
}

solution& solution::operator ()()
{
    for (int n = 0; n < N_; n++) {
        for (int m = 0; m < M_; m++) {
            if (field_[n][m] == '.') {
                calculate(n, m);
            }
        }
    }
    return *this;
}

inline void solution::calculate(int n, int m)
{
    int8_t cnt(0);
    for (int r = std::max(0, n - 1); r <= std::min(N_ - 1, n + 1); r++) {
        for (int c = std::max(0, m - 1); c <= std::min(M_ - 1, m + 1); c++) {
            if (r == n && c == m) {
                continue;
            }
            if (field_[r][c] == '*') {
                cnt++;
            }
        }
    }
    field_[n][m] = cnt + '0';
}

}

void U10189::operator()() const
{
    solution sol;
    uint32_t nField(0);
    while (std::cin >> sol && sol) {
        std::cout << "Field #" << (++nField) << ":" << std::endl;
        std::cout << sol() << std::endl;
    }
}
