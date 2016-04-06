#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10315.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10315 instance;
    instance();
}

namespace
{
typedef std::pair<char, char> card;

enum class hand_value
{
    HighCard, Pair, TwoPairs, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush
};

enum class card_value : char
{
    _2 = '2', _3 = '3', _4 = '4', _5 = '5', _6 = '6', _7 = '7', _8 = '8', _9 = '9', T = 'T', J = 'J', Q = 'Q', K = 'K', A = 'A'
};

int to_number (const card_value c)
{
    switch (char(c)) {
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return char(c) - '0';
    case 'T':
        return 10;
    case 'J':
        return 11;
    case 'Q':
        return 12;
    case 'K':
        return 13;
    case 'A':
        return 14;
    default:
        return -1;
    }
}

bool operator < (const card_value c1, const card_value c2)
{
    return to_number(c1) < to_number(c2);
}

int operator - (const card_value c1, const card_value c2)
{
    return to_number(c1) - to_number(c2);
}

class hand
{
    std::vector<card> hand_;
    hand_value value_;
    card_value high_;
public:
    hand () : hand_(), value_(hand_value::HighCard), high_(card_value::_2)
    {
        hand_.reserve(5);
    }

    friend std::istream& operator >> (std::istream& in, hand& h);

    friend std::ostream& operator << (std::ostream& out, const hand& h);

    friend bool operator == (const hand& left, const hand& right)
    {
        if (left.value_ == right.value_) {
            if (left.high_ == right.high_) {
                switch (left.value_) {
                case hand_value::HighCard:
                    if (card_value(left.hand_[1].first) == card_value(right.hand_[1].first)) {
                        if (card_value(left.hand_[2].first) == card_value(right.hand_[2].first)) {
                            if (card_value(left.hand_[3].first) == card_value(right.hand_[3].first)) {
                                return card_value(left.hand_[4].first) == card_value(right.hand_[4].first);
                            }
                        }
                    }
                case hand_value::Pair:
                    if (card_value(left.hand_[2].first) == card_value(right.hand_[2].first)) {
                        if (card_value(left.hand_[3].first) == card_value(right.hand_[3].first)) {
                            return card_value(left.hand_[4].first) == card_value(right.hand_[4].first);
                        }
                    }
                case hand_value::TwoPairs:
                    if (card_value(left.hand_[2].first) == card_value(right.hand_[2].first)) {
                        return card_value(left.hand_[4].first) == card_value(right.hand_[4].first);
                    }
                default:
                    break;
                }
            }
        }
        return false;
    }

    friend bool operator < (const hand& left, const hand& right)
    {
        if (left.value_ == right.value_) {
            return left.high_ < right.high_;
        }
        return left.value_ < right.value_;
    }

private:
    void evaluate();
    bool is_straight();
};

class solution
{
    hand white_;
    hand black_;
public:
    solution() :white_(), black_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol)
    {
        if (in) {
            in >> sol.black_ >> sol.white_;
        }
        return in;
    }

    friend std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        out << sol.black_ << " vs " << sol.white_ << std::endl;
        if (sol.black_ == sol.white_) {
            out << "Tie.";
        } else if (sol.black_ < sol.white_) {
            out << "White wins.";
        } else {
            out << "Black wins.";
        }
        return out;
    }

private:
};

std::istream& operator >> (std::istream& in, hand& h)
{
    h.hand_.clear();
    std::generate_n(std::back_inserter(h.hand_), 5, [&]() {
        card c;
        in >> c.first >> c.second;
        return c;
    });
    if (in) {
        h.evaluate();
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const hand& h)
{
    std::for_each(h.hand_.begin(), h.hand_.end(), [&](auto c) {
        out << c.first << c.second << ' ';
    });
    return out;
}

void hand::evaluate()
{
    bool same_suit = (hand_[0].second == hand_[1].second) && (hand_[1].second == hand_[2].second) && (hand_[2].second == hand_[3].second) && (hand_[3].second == hand_[4].second);
    std::sort(hand_.begin(), hand_.end(), [](auto c1, auto c2) {
        return card_value(c2.first) < card_value(c1.first);
    });
    high_ = card_value(hand_[0].first);
    value_ = hand_value::HighCard;
    std::map<card_value, int> temp;
    std::for_each(hand_.begin(), hand_.end(), [&](auto c) {
        temp[card_value(c.first)]++;
    });
    std::vector<std::pair<card_value, int>> distribution(temp.begin(), temp.end());
    std::sort(distribution.begin(), distribution.end(), [](auto d1, auto d2) {
        return d2.second < d1.second;
    });
    switch (distribution[0].second) {
    case 4:
        value_ = hand_value::FourOfAKind;
        high_ = distribution[0].first;
        break;
    case 3:
        if (distribution.size() == 2) {
            value_ = hand_value::FullHouse;
        } else {
            value_ = hand_value::ThreeOfAKind;
        }
        high_ = distribution[0].first;
        break;
    case 2:
        if (distribution[1].second == 2) {
            value_ = hand_value::TwoPairs;
            high_ = std::max(distribution[0].first, distribution[1].first);
        } else {
            value_ = hand_value::Pair;
            high_ = distribution[0].first;
        }
        break;
    default:
        if (is_straight()) {
            if (same_suit) {
                value_ = hand_value::StraightFlush;
            } else {
                value_ = hand_value::Straight;
            }
        } else if (same_suit) {
            value_ = hand_value::Flush;
        }
        break;
    }
}

bool hand::is_straight()
{
    bool straight (true);
    for (size_t i = 1; straight && i <= 4; i++) {
        if (card_value(hand_[i - 1].first) - card_value(hand_[i].first) != 1) {
            straight = false;
        }
    }
    return straight;
}

}

void U10315::operator()()
{
    solution sol;
    while (std::cin >> sol) {
        std::clog << sol << std::endl;
    }
}
