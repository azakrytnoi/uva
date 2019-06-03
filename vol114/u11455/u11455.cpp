#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11455.h"

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
    U11455 instance;
    instance();
}

namespace {
    enum class category
    {
        banana, quadrangle, rectangle, square
    };
    class solution {
        int32_t s1_, s2_, s3_, s4_;
        category result_;
    public:
        solution() : s1_(), s2_(), s3_(), s4_(), result_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.s1_ >> sol.s2_ >> sol.s3_ >> sol.s4_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        switch (sol.result_)
        {
        case category::square:
            out << "square";
            break;

        case category::rectangle:
            out << "rectangle";
            break;

        case category::quadrangle:
            out << "quadrangle";
            break;

        default:
            out << "banana";
            break;
        }

        return out;
    }

    solution& solution::operator()()
    {
        result_ = category::banana;

        if (s1_ != 0 && s2_ != 0 && s3_ != 0 && s4_ != 0)
        {
            if (s1_ == s2_ && s2_ == s3_ && s3_ == s4_)
            {
                result_ = category::square;
            }
            else if ((s1_ == s3_ && s2_ == s4_)
                     || (s1_ == s2_ && s3_ == s4_)
                     || (s1_ == s4_ && s3_ == s2_))
            {
                result_ = category::rectangle;
            }
            else if ((s1_ <= (s2_ + s3_ + s4_))
                     && (s2_ <= (s1_ + s3_ + s4_))
                     && (s3_ <= (s1_ + s2_ + s4_))
                     && (s4_ <= (s1_ + s2_ + s3_)))
            {
                result_ = category::quadrangle;
            }
        }

        return *this;
    }
}

void U11455::operator()() const
{
    int32_t N;
    std::cin >> N;
    solution sol;

    while (N--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
