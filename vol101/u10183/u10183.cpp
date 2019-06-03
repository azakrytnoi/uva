#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10183.h"

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
    U10183 instance;
    instance();
}

namespace {

    class solution {
        std::vector<std::string> fibs_;
        std::string s1_, s2_;
        size_t count_;
    public:
        solution() : fibs_(), s1_(), s2_(), count_()
        {
            fibs_.reserve(502);
            fibs_.push_back("1");
            fibs_.push_back("2");
            std::generate_n(std::back_inserter(fibs_), 500, [&]()
            {
                return solution::add(fibs_[fibs_.size() - 2], fibs_[fibs_.size() - 1]);
            });
        }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !((s1_ == "0") && (s2_ == "0"));
        }
        solution& operator()();

    private:
        static std::string add(const std::string& lhs, const std::string& rhs);
        static bool less(const std::string& lhs, const std::string& rhs);
        static std::string normalize(const std::string& src, size_t length);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.s1_ >> sol.s2_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.count_;
        return out;
    }

    solution& solution::operator()()
    {
        count_ = std::accumulate(fibs_.begin(), fibs_.end(), 0ull, [&](size_t current, const std::string & fib)
        {
            return current + (solution::less(s1_, fib) && solution::less(fib, s2_) ? 1 : 0);
        });
        return *this;
    }

    std::string solution::add(const std::string& lhs, const std::string& rhs)
    {
        size_t len(std::max(lhs.length(), rhs.length()));
        const std::string s1(solution::normalize(lhs, len));
        const std::string s2(solution::normalize(rhs, len));
        std::string res ("");
        res.reserve(len + 1);
        int32_t  carry = 0;

        for (int32_t i = static_cast<int32_t>(len - 1); i >= 0; i--)
        {
            int32_t sum = carry + (s1[i] - '0') + (s2[i] - '0');
            carry = sum / 10;
            sum = sum % 10;
            res = (char)(sum + '0') + res;
        }

        if (carry)
        {
            res  = '1' + res;
        }

        return res;
    }

    bool solution::less(const std::string& lhs, const std::string& rhs)
    {
        size_t len(std::max(lhs.length(), rhs.length()));
        const std::string s1(solution::normalize(lhs, len));
        const std::string s2(solution::normalize(rhs, len));
        auto is1 (s1.cbegin());
        auto is2 (s2.cbegin());

        for (; is1 != s1.cend() && is2 != s2.cend() && *is1 == *is2; ++is1, ++is2) { }

        return is1 == s1.cend() ? true : *is1 < *is2;
    }

    std::string solution::normalize(const std::string& src, size_t length)
    {
        std::string res(length, '0');
        std::copy(src.rbegin(), src.rend(), res.rbegin());
        return res;
    }
}

void U10183::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
