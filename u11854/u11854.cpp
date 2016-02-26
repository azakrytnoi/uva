#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)

#include "u11854.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <numeric>

void U11854::operator()()
{
    std::vector<uint32_t> triangle;
    triangle.resize(3);
    while (std::cin) {
        std::cin >> triangle[0] >> triangle[1] >> triangle[2];
        if (std::accumulate(triangle.begin(), triangle.end(), 0) == 0) break;
        std::sort(triangle.begin(), triangle.end());
        std::cout << (triangle[0] * triangle[0] + triangle[1] * triangle[1] == triangle[2] * triangle[2] ? "right" : "wrong") << std::endl;
    }
}
