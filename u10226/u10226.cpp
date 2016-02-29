#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u10226.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <iterator>
#include <algorithm>

namespace
{
std::ostream& operator << (std::ostream& out, const std::pair<std::string, double>& pair)
{
    out << pair.first << " " << std::fixed << std::setprecision(4) << pair.second;
    return out;
}
}


U10226::U10226()
{
}

U10226::~U10226()
{
}

void U10226::operator()()
{
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    std::map<std::string, int> species;
    while (N--) {
        species.clear();
        int total(0);
        while (std::getline(std::cin, line) && !line.empty()) {
            species[line]++;
            total++;
        }
        std::for_each(species.begin(), species.end(), [&](auto specie) {
            std::cout << std::make_pair(specie.first, double(specie.second) / total * 100.0) << std::endl;
        });
        std::cout << std::endl;
    }
}