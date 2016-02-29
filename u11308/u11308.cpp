#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u11308.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>

void U11308::operator()()
{
    int N;
    std::cin >> N;
    std::string line;
    while (N--) {
        std::getline(std::cin, line);
        std::map<std::string, int> stock;
        std::vector<std::pair<std::string, int>> requests;
        std::string backery;
        std::getline(std::cin, backery);
        int n_stock, n_recipes, budget;
        std::cin >> n_stock >> n_recipes >> budget;
        while(n_stock--) {
            int price;
            std::cin >> line >> price;
            stock[line] = price;
        }
        requests.reserve(n_recipes);
        while (n_recipes--) {
            std::getline(std::cin, line);
            std::getline(std::cin, line);
            std::pair<std::string, int> request = std::make_pair(line, 0);
            std::cin >> n_stock;
            while (n_stock--) {
                int qty;
                std::cin >> line >> qty;
                request.second += stock[line] * qty;
            }
            if (request.second <= budget) {
                requests.push_back(request);
            }
        }
        std::transform(backery.begin(), backery.end(), backery.begin(), [](auto ch) {
            return std::toupper(ch);
        });
        std::cout << backery << std::endl;
        if (requests.empty()) {
            std::cout << "Too expensive!" << std::endl;
        } else {
            std::sort(requests.begin(), requests.end(), [](auto r1, auto r2) -> bool {
                if (r1.second == r2.second)
                {
                    return r1.first < r2.first;
                }
                return r1.second < r2.second;
            });
            std::for_each (requests.begin(), requests.end(), [](auto r) {
                std::cout << r.first << std::endl;
            });
        }
        std::cout << std::endl;
    }
}