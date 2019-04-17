#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u257.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U257 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : src_(), palingword_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not src_.empty();
        }
        solution_t& operator()();

    private:
        std::string src_;
        bool palingword_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.src_.clear();
        in >> sol.src_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.palingword_)
        {
            out << sol.src_ << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<std::vector<bool>> dp(src_.size(), std::vector<bool>(src_.size(), true));

        for (size_t i = 0; i < src_.size() - 1; ++i)
        {
            dp[i][i + 1] = (src_[i] == src_[i + 1] ? true : false);
        }

        auto check = [&]()
        {
            std::string candidate;
            size_t left (0), right (src_.size() - 1);

            for (size_t len = 2; len < src_.size(); ++len)
            {
                for (size_t i = 0; i + len < src_.size(); ++i)
                {
                    size_t j = i + len;

                    if (src_[i] == src_[j] && dp[i + 1][j - 1])
                    {
                        dp[i][j] = true;

                        if (candidate == "")
                        {
                            candidate = src_.substr(i, len + 1);
                            right = i;
                            left = j;
                        }
                        else if (i > right || j < left)
                        {
                            if (src_.substr(i, len + 1).find(candidate) == std::string::npos)
                            {
                                return true;
                            }
                        }
                    }
                    else
                    {
                        dp[i][j] = false;
                    }
                }
            }

            return false;
        };

        palingword_ = check();
        return *this;
    }

}

void U257::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol();
    }
}
