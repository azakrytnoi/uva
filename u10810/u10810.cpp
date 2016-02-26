#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)

#include "u10810.h"

#include <iostream>

#include "sort.h"

U10810::U10810()
{
}

void U10810::operator()()
{
    int n;
    while (std::cin >> n && n > 0) {
        util::sort<999999999> s(n);
        std::cin >> s;
        std::cout << s.do_sort() << std::endl;
    }
}
