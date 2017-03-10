#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u727.h"

#include <iostream>
#include <string>
#include <stack>
#include <map>

namespace {
    std::map<char, int> op_rank;

    std::string process(const std::string& input)
    {
        std::string result("");
        std::stack <char> op_stack;

        for (size_t i = 0; i < input.size(); i++) {
            switch (input[i]) {
            case '(':
                op_stack.push(input[i]);
                break;

            case ')':
                while (op_stack.top() != '(') {
                    char ch = op_stack.top();
                    op_stack.pop();
                    result += ch;
                }

                op_stack.pop();
                break;

            case '+':
            case '-':
            case '*':
            case '/':
                if (op_stack.empty() || op_stack.top() == '(') {
                    op_stack.push(input[i]);

                } else {
                    while (!op_stack.empty() && op_rank[input[i]] <= op_rank[op_stack.top()]) {
                        char ch = op_stack.top();
                        op_stack.pop();
                        result += ch;
                    }

                    op_stack.push(input[i]);
                }

                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                result += input[i];
                break;

            default:
                break;
            }
        }

        while (!op_stack.empty()) {
            if (op_stack.top() != '(') {
                result += op_stack.top();
            }

            op_stack.pop();
        }

        return result;
    }
}

U727::U727()
{}

U727::~U727()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U727 instance;
    instance();
}
void U727::operator()() const
{
    {
        op_rank['/'] = 3;
        op_rank['*'] = 3;
        op_rank['-'] = 1;
        op_rank['+'] = 1;
    }
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);

    while (N--) {
        std::cout << std::endl;
        std::string input;

        while (std::getline(std::cin, line) && !line.empty()) {
            input += line;
        }

        std::cout << process(input) << std::endl;
    }
}
