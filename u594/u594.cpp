#define UVA_API_EXPORT __declspec(dllexport)

#include "u594.h"

#include <iostream>
#include <cstdint>
#include <algorithm>

U594::U594()
{
}


U594::~U594()
{
}

namespace
{
union {
    int32_t numerical;
    unsigned char bytes[4];

    int32_t flip_endian() {
        std::swap(bytes[0], bytes[3]);
        std::swap(bytes[1], bytes[2]);
        return numerical;
    }
} item;
}

void U594::operator()()
{
    while (std::cin >> item.numerical) {
        std::cout << item.numerical << " converts to ";
        std::cout << item.flip_endian() << std::endl;
    }
}
