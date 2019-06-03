#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u499.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U499 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution() : raw_(), result_() {}

        operator bool ()
        {
            return not raw_.empty();
        }

        solution& operator()()
        {
            std::vector<std::pair<char, uint64_t>> work(raw_.begin(), raw_.end());
            std::map<uint64_t, std::string> reverse;

            for (auto& pair : work)
            {
                reverse[pair.second] += pair.first;
            }

            for (auto& pair : reverse)
            {
                std::sort(pair.second.begin(), pair.second.end());
            }

            result_.assign(reverse.rbegin(), reverse.rend());
            return *this;
        }

        friend std::istream& operator >> (std::istream& in, solution& sol)
        {
            sol.raw_.clear();
            sol.result_.clear();
            std::string line;

            if (std::getline(in, line) && not line.empty())
            {
                std::stringstream sin(line);
                char ch;

                while (sin >> ch)
                {
                    if (std::isalpha(ch))
                    {
                        sol.raw_[ch]++;
                    }
                }
            }

            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const solution& sol)
        {
            out << sol.result_[0].second << " " << sol.result_[0].first;
            return out;
        }

    private:
        std::map<char, uint64_t> raw_;
        std::vector<std::pair<uint64_t, std::string>> result_;
    };


}

void U499::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
