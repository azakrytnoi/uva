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
#include <utility>

#include "card.h"

#ifndef _CONST_FUN
    #define _CONST_FUN constexpr
#endif

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10315 instance;
    instance();
}

namespace {

    typedef T10::rank_t rank_t;

    int to_number(const rank_t c)
    {
        switch (static_cast<char>(c))
        {
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
}

#ifdef _WIN32

bool operator < (const rank_t& c1, const rank_t& c2)
{
    return to_number(c1) < to_number(c2);
}

namespace std {
    template<>
    struct less<rank_t>
    {
        // functor for operator<
        typedef rank_t first_argument_type;
        typedef rank_t second_argument_type;
        typedef bool result_type;

        _CONST_FUN bool operator()(const rank_t& _Left, const rank_t& _Right) const
        {
            // apply operator< to operands
            return (_Left < _Right);
        }
    };
}

#endif

namespace {

#ifndef _WIN32

    bool operator < (const rank_t c1, const rank_t c2)
    {
        return to_number(c1) < to_number(c2);
    }

#endif

    typedef card_t<T10::rank_t> card_t;
    typedef poker::combination_t combination_t;

    int operator - (const rank_t c1, const rank_t c2)
    {
        return to_number(c1) - to_number(c2);
    }

    class hand_t {
        std::vector<card_t> hand_;
        combination_t value_;
        std::vector<std::pair<rank_t, int>> distribution_;

    public:
        hand_t() : hand_(), value_(combination_t::HighCard), distribution_()
        {
            distribution_.reserve(5);
            hand_.reserve(5);
        }

        friend std::istream& operator >> (std::istream& in, hand_t& h);

        friend std::ostream& operator << (std::ostream& out, const hand_t& h);

        friend bool operator == (const hand_t& left, const hand_t& right)
        {
            if (left.value_ == right.value_)
            {
                switch (left.value_)
                {
                case combination_t::Flush:
                case combination_t::StraightFlush:
                    for (size_t idx = 0; idx < left.hand_.size(); idx++)
                    {
                        if (left.hand_[idx].rank_ != right.hand_[idx].rank_)
                        {
                            return false;
                        }
                    }

                    return true;

                default:
                    for (size_t idx = 0; idx < left.distribution_.size(); idx++)
                    {
                        if (left.distribution_[idx].first != right.distribution_[idx].first)
                        {
                            return false;
                        }
                    }

                    return true;
                }
            }

            return false;
        }

        friend bool operator < (const hand_t& left, const hand_t& right)
        {
            if (left.value_ == right.value_)
            {
                switch (left.value_)
                {
                case combination_t::Flush:
                case combination_t::StraightFlush:
                    for (size_t idx = 0; idx < left.hand_.size(); idx++)
                    {
                        if (left.hand_[idx].rank_ != right.hand_[idx].rank_)
                        {
                            return left.hand_[idx].rank_ < right.hand_[idx].rank_;
                        }
                    }

                    return true;

                default:
                    for (size_t idx = 0; idx < left.distribution_.size(); idx++)
                    {
                        if (left.distribution_[idx].first != right.distribution_[idx].first)
                        {
                            return left.distribution_[idx].first < right.distribution_[idx].first;
                        }
                    }

                    return true;
                }
            }

            return left.value_ < right.value_;
        }

    private:
        void evaluate();
        bool is_straight();
    };

    class solution {
        hand_t white_;
        hand_t black_;
    public:
        solution() : white_(), black_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol)
        {
            if (in)
            {
                in >> sol.black_ >> sol.white_;
            }

            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol)
        {
            out << sol.black_ << " vs " << sol.white_ << std::endl;

            if (sol.black_ == sol.white_)
            {
                out << "Tie.";

            }
            else if (sol.black_ < sol.white_)
            {
                out << "White wins.";

            }
            else
            {
                out << "Black wins.";
            }

            return out;
        }

    private:
    };

    std::istream& operator >> (std::istream& in, hand_t& h)
    {
        h.hand_.clear();
        std::istream_iterator<card_t> cin(in);
        std::copy_n(cin, 5, std::back_inserter(h.hand_));

        if (in)
        {
            h.evaluate();
        }

        return in;
    }

    void hand_t::evaluate()
    {
        bool same_suit = (hand_[0].suit_ == hand_[1].suit_)
                         && (hand_[1].suit_ == hand_[2].suit_)
                         && (hand_[2].suit_ == hand_[3].suit_)
                         && (hand_[3].suit_ == hand_[4].suit_);
        bool straight(is_straight());

        if (same_suit)
        {
            if (straight)
            {
                value_ = combination_t::StraightFlush;

            }
            else
            {
                value_ = combination_t::Flush;
            }

        }
        else
        {
            value_ = combination_t::HighCard;
            std::map<rank_t, int> temp;
            std::for_each(hand_.begin(), hand_.end(), [&](auto c)
            {
                temp[c.rank_]++;
            });
            distribution_.assign(temp.begin(), temp.end());
            std::sort(distribution_.begin(), distribution_.end(), [](auto d1, auto d2)
            {
                if (d2.second == d1.second)
                {
                    return d2.first < d1.first;
                }

                return d2.second < d1.second;
            });

            switch (distribution_[0].second)
            {
            case 4:
                value_ = combination_t::FourOfAKind;
                break;

            case 3:
                if (distribution_.size() == 2)
                {
                    value_ = combination_t::FullHouse;

                }
                else
                {
                    value_ = combination_t::ThreeOfAKind;
                }

                break;

            case 2:
                if (distribution_[1].second == 2)
                {
                    value_ = combination_t::TwoPairs;

                }
                else
                {
                    value_ = combination_t::OnePair;
                }

                break;

            default:
                if (straight)
                {
                    value_ = combination_t::Straight;
                }

                break;
            }
        }
    }

    bool hand_t::is_straight()
    {
        std::sort(hand_.begin(), hand_.end(), [](auto c1, auto c2)
        {
            return c2.rank_ < c1.rank_;
        });
        bool straight(true);

        for (size_t i = 1; straight && i <= 4; i++)
        {
            if (hand_[i - 1].rank_ - hand_[i].rank_ != 1)
            {
                straight = false;
            }
        }

        return straight;
    }

    std::ostream& operator<<(std::ostream& out, const combination_t val)
    {
        static const std::map<combination_t, std::string> names (
        {
            {combination_t::HighCard, "High Card"},
            {combination_t::OnePair, "One Pair"},
            {combination_t::TwoPairs, "Two Pairs"},
            {combination_t::ThreeOfAKind, "Three of a Kind"},
            {combination_t::Straight, "Straight"},
            {combination_t::Flush, "Flush"},
            {combination_t::FullHouse, "Full House"},
            {combination_t::FourOfAKind, "Four of a Kind"},
            {combination_t::StraightFlush, "Straight Flush"}
        });
        out << names.find(val)->second;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const hand_t& hand)
    {
        out << hand.value_ << ": [ ";
        std::ostream_iterator<card_t> cout(out, " ");
        std::copy(hand.hand_.begin(), hand.hand_.end(), cout);
        out << ']';
        return out;
    }
}

void U10315::operator()() const
{
    solution sol;

    while (std::cin >> sol)
    {
        std::cout << sol << std::endl;
    }
}
