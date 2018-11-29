#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11221.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cctype>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11221 instance;
    instance();
}

void U11221::operator()() const
{
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    int caseNo(0);

    while (N--) {
        std::cout << "Case #" << (++caseNo) << ":" << std::endl;
        std::getline(std::cin, line);
        std::string palindrome;
        palindrome.reserve(line.length());
        std::copy_if(line.begin(), line.end(), std::back_inserter(palindrome), [](auto ch) {
            return std::isalpha(ch);
        });
        size_t sqSize = size_t(std::sqrt(palindrome.length()));

        if (sqSize * sqSize == palindrome.length()) {
            std::string rev(palindrome);
            std::reverse(rev.begin(), rev.end());

            if (rev == palindrome) {
                std::cout << sqSize << std::endl;
                continue;
            }
        }

        std::cout << "No magic :(" << std::endl;
    }
}
