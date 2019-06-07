#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11111.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <stack>
#include <sstream>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11111 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : src_()
        {}

        ~solution()
        {}

        solution& operator << (const std::string& input);
        operator bool() const;

    private:
        std::vector<int> src_;
    };

    solution& solution::operator<<(const std::string& input)
    {
        src_.clear();
        std::stringstream iss(input);
        std::istream_iterator<int> iit(iss);
        std::copy(iit, std::istream_iterator<int>(), std::back_inserter(src_));
        return *this;
    }

    solution::operator bool() const
    {
        std::stack<int> storage;
        std::stack<int> summs;
        int sum(0);

        for (auto it = src_.begin(); it != src_.end(); ++it)
        {
            if (*it < 0)
            {
                storage.push(*it);
                summs.push(sum);
                sum = 0;
            }
            else
            {
                if (!storage.empty())
                {
                    if (((storage.top() + *it) != 0) || (*it <= sum))
                    {
                        break;
                    }

                    if (!summs.empty())
                    {
                        sum = summs.top();
                        summs.pop();
                    }

                    sum += *it;
                    storage.pop();
                }
                else
                {
                    return false;
                }
            }
        }

        return storage.empty();
    }

}

void U11111::operator()() const
{
    solution sol;
    std::string line;

    while (std::getline(std::cin, line))
    {
        sol << line;

        if (sol)
        {
            std::cout << ":-) Matrioshka!" << std::endl;
        }
        else
        {
            std::cout << ":-( Try again." << std::endl;
        }
    }
}
