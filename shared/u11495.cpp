#define UVA_API_EXPORT __declspec(dllexport)

#include "u11495.h"

#include "sort.h"

#include <string>
#include <iostream>
#include <sstream>

U11495::U11495()
{
}


U11495::~U11495()
{
}

void U11495::operator()()
{
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream in(line);
        int n;
        in >> n;
        if (n == 0) break;
        else {
            util::sort<100000> s(n);
            in >> s;
            std::cout << ((s.do_sort() & 0x01) ? "Marcelo" : "Carlos") << std::endl;
        }
    }
}
