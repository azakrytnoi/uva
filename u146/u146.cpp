#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u146.h"

#include <string>
#include <iostream>
#include <algorithm>

U146::U146()
{}

U146::~U146()
{}
extern "C"
{   UVA_API_EXPORT void __cdecl invoke(); }
void __cdecl invoke()
{
    U146 instance;
    instance();
}
void U146::operator()() const
{
    std::string line;
    while (std::getline(std::cin, line)) {
        if ("#" == line) {
            break;
        }

        if (std::next_permutation(line.begin(), line.end())) {
            std::cout << line << std::endl;
        } else {
            std::cout << "No Successor" << std::endl;
        }
    }
}
