#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#endif

#include "u10219.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

U10219::U10219()
{
}

typedef long long LL;

void U10219::operator()()
{
    LL n, k;

    while ( std::cin >> n >> k ) {
        double digit = 0;

        if ( k > n - k ) {
            for ( LL i = k + 1; i <= n; i++ ) {
                digit += (std::log10 (i) - std::log10 (n - i + 1));
            }
        } else {
            for ( LL i = n - k + 1; i <= n; i++ ) {
                digit += (std::log10 (i) - std::log10 (n - i + 1));
            }
        }

        digit = floor (digit) + 1;

        std::cout << std::fixed << std::setprecision(0) << digit << std::endl;
    }
}
