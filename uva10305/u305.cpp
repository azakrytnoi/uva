#include "u305.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

namespace
{
int32_t circle_count (int32_t k, int32_t m)
{
    int32_t size = 2 * k;

    int32_t current ( (m - 1) % size), i (0);
    for (i = 0; i < 2 * k; i++) {
        if (current < k) break;
        size--;
        current = (current + m - 1) % (size);
    }

    return i;
}
}

void U305::operator()()
{
    std::vector<int32_t> answers;
    answers.reserve(15);
    answers.push_back(0);
    std::generate_n(std::back_inserter(answers), 15, [&]() -> int32_t {
        int32_t probe(1);
        int32_t k = answers.size();
        for (; circle_count(k, ++probe) != k;);
        return probe;
    });

    std::istream_iterator<int32_t> iin(std::cin);
    std::ostream_iterator<int32_t> iout(std::cout, "\n");
    std::transform(iin, std::istream_iterator<int32_t>(), iout, [&](auto k) -> int32_t {
        return answers[k];
    });
}
