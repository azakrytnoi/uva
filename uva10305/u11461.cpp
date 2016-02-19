#include "u11461.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

void U11461::operator()()
{
    std::vector<uint32_t> squares;
    squares.reserve(400);
    for(uint32_t i = 0; i * i <= 100000; ++i) {
        squares.push_back(i * i);
    }
    int32_t a, b;
    while (std::cin >> a >> b && !(a == 0 && b == 0)) {
        auto lit = std::lower_bound(squares.begin(), squares.end(), a);
        auto uit = std::upper_bound(squares.begin(), squares.end(), b);
        auto dist = std::distance(lit, uit);
        std::cout << dist << std::endl;
    }
}
