#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11136.h"

#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>
#include <numeric>
#include <iterator>

U11136::U11136()
{}

U11136::~U11136()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11136 instance;
    instance();
}
void U11136::operator()()
{
    int N;
    std::string line;
    std::multiset<int> bills;

    while ((std::cin >> N) && N > 0)
    {
        std::getline(std::cin, line);
        bills.clear();
        int total(0);

        while (N--)
        {
            std::getline(std::cin, line);
            std::stringstream sin(line);
            int n;
            sin >> n;

            if (n > 0)
            {
                std::istream_iterator<int> iin(sin);
                std::for_each(iin, std::istream_iterator<int>(), [&](auto item)
                {
                    bills.insert(item);
                });
            }

            total += (*bills.rbegin() - *bills.begin());
            bills.erase(bills.begin());
            bills.erase((--bills.end()));
        }

        std::cout << total << std::endl;
    }
}