#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u673.h"

#include <iostream>
#include <stack>
#include <string>
#include <sstream>

U673::U673()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U673 instance;
    instance();
}
void U673::operator()() const
{
    std::string line;
    int N;
    std::cin >> N;
    std::getline(std::cin, line);

    while (N--)
    {
        std::getline(std::cin, line);
        std::stringstream iss(line);
        std::stack<char> check;
        char ch;
        bool valid(true);

        while (valid && iss >> ch)
        {
            switch (ch)
            {
            case '[':
            case '(':
                check.push(ch);
                break;

            case ']':
                if (!check.empty() && '[' == check.top())
                {
                    check.pop();
                }
                else
                {
                    valid = false;
                }

                break;

            case ')':
                if (!check.empty() && '(' == check.top())
                {
                    check.pop();

                }
                else
                {
                    valid = false;
                }

                break;

            default:
                break;
            }
        }

        valid &= check.empty();
        std::cout << (valid ? "Yes" : "No") << std::endl;
    }
}
