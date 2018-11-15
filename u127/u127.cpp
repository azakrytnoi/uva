#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u127.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>

namespace {
typedef std::pair<char, char> card;

bool can(card c1, card c2)
{
    return (c1.first == c2.first || c1.second == c2.second);
}
}

U127::U127()
{}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U127 instance;
    instance();
}
void U127::operator()() const
{
    std::vector <std::stack <card>> cards;
    std::string word;

    while (std::cin >> word && word != "#") {
        cards.clear();
        std::stack <card> s;
        s.push(card(word[0], word[1]));
        cards.push_back(s);

        for (int i = 0; i < 51; i++) {
            std::cin >> word;
            std::stack <card> s;
            s.push(card(word[0], word[1]));
            cards.push_back(s);
        }

        for (size_t i = 0; i < cards.size(); i++) {
            if (i >= 3 && can(cards[i].top(), cards[i - 3].top())) {
                cards[i - 3].push(cards[i].top());
                cards[i].pop();

                if (cards[i].empty()) {
                    cards.erase(cards.begin() + i);
                }

                i -= 4;

            } else if (i >= 1 && can(cards[i].top(), cards[i - 1].top())) {
                cards[i - 1].push(cards[i].top());
                cards[i].pop();

                if (cards[i].empty()) {
                    cards.erase(cards.begin() + i);
                }

                i -= 2;
            }
        }

        std::cout << cards.size() << " pile" << (cards.size() == 1 ? "" : "s") << " remaining: ";
        std::ostream_iterator<size_t> oi(std::cout, " ");
        std::transform(cards.begin(), cards.end(), oi, [](const std::stack<card>& s) {
            return s.size();
        });
        std::cout << std::endl;
    }
}
