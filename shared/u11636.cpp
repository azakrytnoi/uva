#define UVA_API_EXPORT __declspec(dllexport)

#include "u11636.h"

#include <iostream>

U11636::U11636()
{
}


U11636::~U11636()
{
}

void U11636::operator()()
{
    int lines;
    int case_no(0);
    while (std::cin >> lines && lines > 0) {
        int pastes(0);
        lines -= 1;
        while (lines > 0) {
            lines >>= 1;
            pastes++;
        }
        std::cout << "Case " << (++case_no) << ": " << pastes << std::endl;
    }
}
