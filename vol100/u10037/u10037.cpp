#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10037.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <deque>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10037 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution();

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool();

    private:
        std::deque<uint16_t> left_;
        std::deque<uint16_t> right_;
        std::stringstream sout_;
        uint64_t total_;
    };

    solution::solution() : left_(), right_(), sout_(), total_()
    {}

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.left_.clear();
        sol.right_.clear();
        sol.sout_.str(std::string());
        sol.total_ = 0;
        size_t n;

        if (in >> n)
        {
            std::generate_n(std::back_inserter(sol.left_), n, [&]()
            {
                uint16_t tmp;
                in >> tmp;
                return tmp;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.total_ << std::endl << sol.sout_.str() << std::endl;
        return out;
    }

    solution::operator bool()
    {
        if (left_.empty())
        {
            return false;
        }

        std::sort(left_.begin(), left_.end());

        while (true)
        {
            auto A = left_[0];

            if (left_.size() == 1)
            {
                sout_ << A;
                total_ += A;
                break;
            }

            auto B = left_[1];

            if (left_.size() == 2)
            {
                sout_ << A << ' ' << B;
                total_ += B;
                break;
            }

            if (left_.size() == 3)
            {
                sout_ << A << ' ' << left_[2] << std::endl << A << std::endl << A << ' ' << B;
                total_ += A + B + left_[2];
                break;
            }

            auto Z = left_.back();
            left_.pop_back();
            auto Y = left_.back();
            left_.pop_back();

            if (A + Y < B * 2)
            {
                sout_ << A << " " << Z << std::endl << A << std::endl << A << " " << Y << std::endl << A << std::endl;
                total_ += Z + 2 * A + Y;
            }
            else
            {
                sout_ << A << " " << B << std::endl << A << std::endl << Y << " " << Z << std::endl << B << std::endl;
                total_ += 2 * B + A + Z;
            }
        }

        return true;
    }
}

void U10037::operator()() const
{
    int N;
    std::cin >> N;
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol << std::endl;
    }
}
