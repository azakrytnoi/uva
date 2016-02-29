#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u400.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <limits>
#include <iomanip>
#include <ios>

void U400::operator()()
{
    uint32_t N;
    std::vector<std::string> names;
    std::string line;
    std::string separator(60, '-');
    while (std::cin >> N && N > 0) {
        std::cout << separator << std::endl;
        names.clear();
        names.reserve(N);
        std::getline(std::cin, line);
        size_t longest(std::numeric_limits<size_t>::min());
        std::generate_n(std::back_inserter(names), N, [&]() -> std::string {
            std::getline(std::cin, line);
            longest = std::max(longest, line.size());
            return line;
        });
        std::sort(names.begin(), names.end());
        size_t ncolumns (std::max(60 / longest, size_t(1)));
        while ((ncolumns -1) * (longest + 2) + longest > 60) {
            ncolumns--;
        }
        size_t nrows (names.size() / ncolumns);
        if (nrows * ncolumns < names.size()) {
            nrows++;
        }
        for (size_t idx = 0; idx < nrows; idx++) {
            for (size_t column = 0; column < ncolumns; column++) {
                if (column * nrows + idx < names.size()) {
                    std::cout.setf(std::ios::left);
                    auto shift = (column == ncolumns - 1)
                                 || ((column + 1) * nrows + idx >= names.size());
                    std::cout << std::setw(longest + (shift ? 0 : 2))
                              << names[column * nrows + idx];
                }
            }
            std::cout << std::endl;
        }
    }
}
