#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u484.h"

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
    U484 instance;
    instance();
}

void U484::operator()() const
{
    std::vector<int> v;
    std::map<int, int> freq;
    int n;

    while (std::cin >> n)
    {
        if ((freq[n]++) == 0)
        {
            v.push_back(n);
        }
    }

    std::for_each(v.begin(), v.end(), [&](auto num)
    {
        std::cout << num << " " << freq[num] << std::endl;
    });
}
