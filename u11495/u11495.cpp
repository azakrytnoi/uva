#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11495.h"

#include "sort.h"

#include <string>
#include <iostream>
#include <sstream>

U11495::U11495()
{}

U11495::~U11495()
{}

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11495 instance;
    instance();
}
void U11495::operator()() const
{
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream in(line);
        int n;
        in >> n;
        if (n == 0) {
            break;
        } else {
            util::sort<100000> s(n);
            in >> s;
            std::cout << ((s.do_sort() & 0x01) ? "Marcelo" : "Carlos") << std::endl;
        }
    }
}
