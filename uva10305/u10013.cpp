#include "u10013.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

void U10013::operator()()
{
    int N;
    std::cin >> N;
    std::vector<int8_t> summ;
    while(N--)
    {
        int M;
        std::cin >> M;
        summ.clear();
        summ.reserve(M + 1);
        summ.push_back(0);
        std::generate_n(std::back_inserter(summ), M, []() {
            int l(0),r(0);
            std::cin >> l >> r;
            return int8_t(l + r);
        });
        for (auto it = summ.rbegin(), end = summ.rend(); it != end; ++it) {
            int8_t rem(*it / 10);
            if (rem > 0) {
                *it %= 10;
                *(it + 1) += rem;
            }
        }
        auto it = summ.begin();
        if (*it == 0) ++it;
        std::ostream_iterator<char> oit(std::cout, "");
        std::transform(it, summ.end(), oit, [](auto d) -> char { return '0' + d;});
        std::cout << std::endl << std::endl;
    }
}