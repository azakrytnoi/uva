#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11764.h"

#include <iostream>

U11764::U11764()
{
}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11764 instance;
    instance();
}

void U11764::operator()() const
{
    int N;
    std::cin >> N;
    int caseNo(0);
    while (N--) {
        int nWalls;
        int hJump(0), lJump(0);
        int current, prev;
        std::cin >> nWalls >> prev;
        while (--nWalls) {
            std::cin >> current;
            (current > prev ? ++hJump : current < prev ? ++lJump : 0);
            prev = current;
        }
        std::cout << "Case " << (++caseNo) << ": " << hJump << " " << lJump << std::endl;
    }
}
