
#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u465.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include <biguint.h>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U465 instance;
    instance();
}

namespace {
    class checker {
    public:
        static std::vector<std::string> check(const std::string& source);
    };
    std::vector<std::string> checker::check(const std::string& source)
    {
        std::vector<std::string> result;
        result.reserve(4);
        result.push_back(source);
        size_t op_position = source.find('+');

        if (op_position == std::string::npos)
        {
            op_position = source.find('*');
        }

        std::string op1 = source.substr(0, op_position - 1);
        std::string op2 = source.substr(op_position + 2, std::string::npos);
        bool op1_long = op1.length() > std::numeric_limits<int>::digits10;
        bool op2_long = op2.length() > std::numeric_limits<int>::digits10;

        if (op1_long)
        {
            result.push_back("first number too big");
        }

        if (op2_long)
        {
            result.push_back("second number too big");
        }

        if (op1_long || op2_long)
        {
            result.push_back("result too big");
        }
        else
        {
            switch (source[op_position])
            {
            case '+':
            {
                math::uint_big_t bi (op1);
                bi += op2;

                if (bi.length() > std::numeric_limits<int>::digits10)
                {
                    result.push_back("result too big");
                }
            }
            break;

            case '*':
                if (op1.length() + op2.length() > std::numeric_limits<int>::digits10)
                {
                    result.push_back("result too big");
                }

                break;

            default:
                break;
            }
        }

        return result;
    }
}

void U465::operator()() const
{
    std::string line;
    std::ostream_iterator<std::string> oit(std::cout, "\n");

    while (std::getline(std::cin, line) && !line.empty())
    {
        std::vector <std::string> result(checker::check(line));
        std::copy(result.begin(), result.end(), oit);
    }
}
