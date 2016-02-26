#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)

#include "u11462.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <cstdint>
#include <algorithm>

U11462::U11462()
{
}


U11462::~U11462()
{
}

void U11462::operator()()
{
    std::vector<uint64_t> data;
    uint64_t N;
    while ((std::cin >> N) && N > 0) {
        data.clear();
        data.reserve(N);
        std::istream_iterator<uint64_t> in(std::cin);
        std::copy_n(in, N, std::back_inserter(data));
        std::sort(data.begin(), data.end());
        std::ostream_iterator<uint64_t> out(std::cout, " ");
        std::copy(data.begin(), data.end(), out);
        std::cout << std::endl;
    }
}
