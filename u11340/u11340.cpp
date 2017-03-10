#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11340.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

U11340::U11340()
{}

U11340::~U11340()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11340 instance;
    instance();
}
void U11340::operator()() const
{
    int N;
    std::cin >> N;

    while (N--) {
        int map_size;
        std::cin >> map_size;
        std::unordered_map<char, int> prices;

        while (map_size--) {
            char letter;
            int price;
            std::cin >> letter >> price;
            prices[letter] = price;
        }

        int article_size;
        std::cin >> article_size;
        int price(0);
        std::string line;
        std::getline(std::cin, line);

        while (article_size--) {
            std::getline(std::cin, line);
            std::for_each(line.begin(), line.end(), [&](char ch) {
                auto it = prices.find(ch);

                if (it != prices.end()) {
                    price += it->second;
                }
            });
        }

        std::cout << std::fixed << std::setprecision(2) << (price / 100.0) << '$' << std::endl;
    }
}
