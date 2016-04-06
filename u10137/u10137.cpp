#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10137.h"

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
    U10137 instance;
    instance();
}

namespace
{

class solution
{
    std::vector<int32_t> expences_;
    int32_t change_;
public:
    solution() : expences_(), change_(0) {}

    friend std::istream& operator >>(std::istream& in, solution& sol)
    {
        int n;
        sol.expences_.clear();
        in >> n;
        if (n > 0) {
            double avg(0);
            sol.expences_.reserve(n);
            std::generate_n(std::back_inserter(sol.expences_), n, [&]() {
                double amount;
                in >> amount;
                int32_t amt (int32_t (amount * 100 + 0.5));
                avg += amt;
                return amt;
            });
            avg = std::ceil(avg / sol.expences_.size());
            int32_t positive(0), negative (0);
            std::for_each(sol.expences_.begin(), sol.expences_.end(), [&](int32_t amt) {
                if (amt > avg) {
                    positive += amt - avg;
                } else {
                    negative += avg - amt;
                }
            });
            sol.change_ = std::min(negative, positive);
            if (sol.change_ == 0){
                sol.change_ = std::ceil(std::max(negative, positive) / 2.0);
            }
        }
        return in;
    }

    friend std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        double result = sol.change_ / 100.0;
        out << "$" << std::fixed << std::setprecision(2) << result;
        return out;
    }

    operator bool() const
    {
        return !expences_.empty();
    }

};

}

void U10137::operator()()
{
    solution sol;
    while (std::cin >> sol && sol) {
        std::cout << sol << std::endl;
    }
}
