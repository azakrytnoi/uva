#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u855.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U855 instance;
    instance();
}


void U855::operator()() const
{
    int N;
    std::cin >> N;
    std::vector<int> streets;
    std::vector<int> avenues;
    while (N--) {
        int temp, nFriends;
        std::cin >> temp >> temp >> nFriends;
        streets.resize(nFriends);
        avenues.resize(nFriends);
        for (auto ita = avenues.begin(), its = streets.begin(); ita != avenues.end(); ++its, ++ita) {
            std::cin >> *its >> *ita;
        }
        std::sort(streets.begin(), streets.end());
        std::sort(avenues.begin(), avenues.end());
        nFriends--;
        nFriends >>= 1;
        std::cout << "(Street: " << streets[nFriends] << ", Avenue: " << avenues[nFriends] << ")" << std::endl;
    }
}
