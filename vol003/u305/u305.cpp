#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u305.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

namespace {
    size_t circle_count(size_t k, size_t m)
    {
        size_t size = 2 * k;

        size_t current((m - 1) % size), i(0);

        for (i = 0; i < 2 * k; i++)
        {
            if (current < k)
            {
                break;
            }

            size--;
            current = (current + m - 1) % (size);
        }

        return i;
    }
}

U305::U305() {}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U305 instance;
    instance();
}
void U305::operator()()
{
    std::vector<size_t> answers;
    answers.reserve(15);
    answers.push_back(0);
    std::generate_n(std::back_inserter(answers), 15, [&]() -> size_t
    {
        size_t probe(1);
        size_t k = answers.size();

        for (; circle_count(k, ++probe) != k;);
        return probe; });

    std::istream_iterator<size_t> iin(std::cin);
    std::ostream_iterator<size_t> iout(std::cout, "\n");
    std::transform(iin, std::istream_iterator<size_t>(), iout, [&](auto k) -> size_t
    {   return answers[k]; });
}