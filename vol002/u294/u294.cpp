#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u294.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U294 instance;
    instance();
}

namespace {

    class solution {
        math::sieve_t<35000> sieve_;
        uint32_t low_;
        uint32_t high_;
        uint32_t result_;
        uint32_t num_factors_;

    public:
        solution() : sieve_(), low_(0), high_(0), result_(0), num_factors_(0) {}

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            in >> sol.low_ >> sol.high_;
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol)
        {
            out << "Between " << sol.low_ << " and " << sol.high_ << ", " << sol.result_ << " has a maximum of " << sol.num_factors_ << " divisors.";
            return out;
        }

        solution& operator ()();

    private:
        uint32_t count_factors(uint32_t num);
    };

    solution& solution::operator ()()
    {
        result_ = 0;
        num_factors_ = 0;

        for (uint32_t num = low_; num <= high_; num++)
        {
            uint32_t factors = count_factors(num);

            if (factors > num_factors_)
            {
                num_factors_ = factors;
                result_ = num;
            }
        }

        return *this;
    }

    uint32_t solution::count_factors(uint32_t num)
    {
        uint32_t result = 1;
        auto factor = sieve_.begin() + 1;

        while ((*factor) * (*factor) <= num)
        {
            uint32_t count(0);

            while (num % (*factor) == 0)
            {
                count++;
                num /= (*factor);
            }

            result *= (count + 1);
            ++factor;
        }

        if (num != 1)
        {
            result *= 2;
        }

        return result;
    }

}

void U294::operator()() const
{
    uint32_t N;
    std::cin >> N;
    solution sol;

    while (N--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
