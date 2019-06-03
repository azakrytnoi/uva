#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10035.h"

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
    U10035 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution() : left_(), right_(), carry_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool();

    private:
        uint64_t left_;
        uint64_t right_;
        uint32_t carry_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.left_ = sol.right_ = sol.carry_ = 0;
        in >> sol.left_ >> sol.right_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.carry_ > 0)
        {
            out << sol.carry_ << " carry operation" << (sol.carry_ > 1 ? "s" : "") << '.';

        }
        else
        {
            out << "No carry operation.";
        }

        return out;
    }

    solution::operator bool()
    {
        if (left_ == 0 && right_ == 0)
        {
            return false;
        }

        uint64_t carry(0);

        while (!(left_ == 0 && right_ == 0))
        {
            carry = ((left_ % 10) + (right_ % 10) + carry) / 10;
            left_ /= 10;
            right_ /= 10;

            if (carry > 0)
            {
                carry_++;
            }
        }

        return true;
    }
}

void U10035::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol << std::endl;
    }
}
