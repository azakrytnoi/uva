#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10026.h"

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
    U10026 instance;
    instance();
}

namespace {

    struct job
    {
        int16_t S_, T_;
        size_t position_;
    };

    std::istream& operator >> (std::istream& in, job& j)
    {
        in >> j.T_ >> j.S_;
        return in;
    }

    class solution {
    public:
        solution() : jobs_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool();

    private:
        std::vector<job> jobs_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.jobs_.clear();
        int16_t n;
        in >> n;

        if (in)
        {
            sol.jobs_.reserve(n);
            std::generate_n(std::back_inserter(sol.jobs_), n, [&]() -> job
            {
                job j;
                in >> j;
                j.position_ = sol.jobs_.size() + 1;
                // cppcheck-suppress returnReference
                return j;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        std::ostream_iterator<size_t> iout(out, " ");
        std::transform(sol.jobs_.begin(), sol.jobs_.end(), iout, [](const job & j)
        {
            return j.position_;
        });
        out.seekp(-1, std::ios::end);
        out << std::endl;
        return out;
    }

    solution::operator bool()
    {
        std::sort(jobs_.begin(), jobs_.end(), [](job & a, job & b)
        {
            double r1 = double(a.S_) / a.T_;
            double r2 = double(b.S_) / b.T_;

            if (r1 != r2)
            {
                return r1 > r2;
            }

            return a.position_ < b.position_;
        });
        return !jobs_.empty();
    }

}

void U10026::operator()() const
{
    int dummy;
    std::cin >> dummy;
    solution sol;

    while (true)
    {
        std::cin >> sol;

        if (sol)
        {
            std::cout << sol << std::endl;

        }
        else
        {
            break;
        }
    }
}
