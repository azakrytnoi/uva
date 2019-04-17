#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u594.h"

#include <iostream>
#include <cstdint>
#include <algorithm>

U594::U594()
{}


U594::~U594()
{}

namespace {
    union
    {
        int32_t numerical;
        unsigned char bytes[4];

        int32_t flip_endian()
        {
            std::swap(bytes[0], bytes[3]);
            std::swap(bytes[1], bytes[2]);
            return numerical;
        }
    } item;
}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U594 instance;
    instance();
}
void U594::operator()() const
{
    while (std::cin >> item.numerical)
    {
        std::cout << item.numerical << " converts to ";
        std::cout << item.flip_endian() << std::endl;
    }
}
