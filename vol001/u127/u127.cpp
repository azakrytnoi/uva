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

#include "card.h"


namespace {
    typedef card_t<T10::rank_t> card_t;

    bool can(const card_t& c1, const card_t& c2)
    {
        return (c1.rank_ == c2.rank_ || c1.suit_ == c2.suit_);
    }

    void evaluate()
    {
        std::vector <std::stack <card_t>> cards;
        std::string word;

        while (std::cin >> word && word != "#")
        {
            cards.clear();
            std::stack <card_t> s;
            s.push(card_t(word[0], word[1]));
            cards.push_back(s);

            for (int i = 0; i < 51; i++)
            {
                std::cin >> word;
                std::stack <card_t> s;
                s.push(card_t(word[0], word[1]));
                cards.push_back(s);
            }

            for (size_t i = 0; i < cards.size(); i++)
            {
                if (i >= 3 && can(cards[i].top(), cards[i - 3].top()))
                {
                    cards[i - 3].push(cards[i].top());
                    cards[i].pop();

                    if (cards[i].empty())
                    {
                        cards.erase(cards.begin() + i);
                    }

                    i -= 4;

                }
                else if (i >= 1 && can(cards[i].top(), cards[i - 1].top()))
                {
                    cards[i - 1].push(cards[i].top());
                    cards[i].pop();

                    if (cards[i].empty())
                    {
                        cards.erase(cards.begin() + i);
                    }

                    i -= 2;
                }
            }

            std::cout << cards.size() << " pile" << (cards.size() == 1 ? "" : "s") << " remaining: ";
            auto cbeg = cards.begin();
            std::cout << cbeg->size();
            ++cbeg;
            std::for_each(cbeg, cards.end(), [&](const std::stack<card_t>& s)
            {
                std::cout << ' ' << s.size();
            });
            std::cout << std::endl;
        }
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
    evaluate();
}
