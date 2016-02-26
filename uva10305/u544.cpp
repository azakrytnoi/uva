#include "u544.h"

#include <iostream>
#include <map>
#include <string>
#include <algorithm>

U544::U544()
{
}


U544::~U544()
{
}

void U544::operator()()
{
    int n, r;
    int cases = 0;

    while (std::cin >> n >> r) {
        if (n == 0 && r == 0) break;

        std::map <std::string, int> cityIndex;
        std::string first, second;
        int cost;
        int index = 1;

        int d[205][205];

        for (int i = 0; i < 205; i++) {
            for (int j = 0; j < 205; j++) d[i][j] = -1;
            d[i][i] = 0;
        }

        for (int i = 0; i < r; i++) {
            std::cin >> first >> second >> cost;
            if (!cityIndex[first]) cityIndex[first] = index++;
            if (!cityIndex[second]) cityIndex[second] = index++;

            d[cityIndex[first]][cityIndex[second]] = cost;
            d[cityIndex[second]][cityIndex[first]] = cost;
        }

        for (int k = 1; k <= n; k++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    d[i][j] = d[j][i] = std::max(d[i][j], std::min(d[i][k], d[k][j]));
                }
            }
        }


        std::string source, dest;
        std::cin >> source >> dest;
        std::cout << "Scenario #" << ++cases << std::endl;
        std::cout << d[cityIndex[source]][cityIndex[dest]] << " tons\n\n";
    }
}
