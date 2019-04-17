#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u488.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U488 instance;
    instance();
}

namespace {
    class solution {
        int32_t amp_, freq_;
        std::stringstream out_;
    public:
        solution() : amp_(), freq_(), out_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !(amp_ == 0 && freq_ == 0);
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.amp_ >> sol.freq_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        for (int32_t i = 0; i < sol.freq_; i++)
        {
            out << sol.out_.str() << std::endl;;
        }

        return out;
    }

    solution& solution::operator()()
    {
        out_.str("");

        for (int32_t i = 1; i <= amp_; i++)
        {
            out_ << std::string(i, static_cast<char>('0' + i)) << std::endl;
        }

        for (int i = amp_ - 1; i >= 1; i--)
        {
            out_ << std::string(i, static_cast<char>('0' + i)) << std::endl;
        }

        return *this;
    }
}

void U488::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--)
    {
        std::cin >> sol;

        if (sol)
        {
            std::cout << sol();
        }
    }
}
