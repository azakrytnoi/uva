#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10810.h"

#include <iostream>

#include "sort.h"

U10810::U10810()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10810 instance;
    instance();
}
void U10810::operator()() const
{
    int n;

    while (std::cin >> n && n > 0) {
        util::sort<999999999> s(n);
        std::cin >> s;
        std::cout << s.do_sort() << std::endl;
    }
}
