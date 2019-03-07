/*
 * card.h
 *
 *  Created on: Mar 7, 2019
 *      Author: aleksey
 */

#pragma once

#include <iostream>
#include <array>
#include <map>

enum class suit_t : char {
	Clubs = 'C', Diamonds = 'D', Hearts = 'H', Spades = 'S'
};

namespace X10 {

enum class rank_t : char {
    A = 'A', _2 = '2', _3 = '3', _4 = '4', _5 = '5', _6 = '6', _7 = '7', _8 = '8', _9 = '9', X = 'X', J = 'J', Q = 'Q', K = 'K'
};

}  // namespace X10

namespace T10 {

enum class rank_t : char {
    A = 'A', _2 = '2', _3 = '3', _4 = '4', _5 = '5', _6 = '6', _7 = '7', _8 = '8', _9 = '9', T = 'T', J = 'J', Q = 'Q', K = 'K'
};

}  // namespace T10

template<typename RANK = X10::rank_t>
class card_t {
public:
	typedef RANK rank_t;
    rank_t rank_;
    suit_t suit_;

    card_t() : rank_(), suit_() {}
    card_t(char rank, char suit) : rank_(static_cast<rank_t>(rank)), suit_(static_cast<suit_t>(suit)) {}
    card_t(rank_t rank, suit_t suit) : rank_(rank), suit_(suit) {}
    card_t(const card_t& rhs) : rank_(rhs.rank_), suit_(rhs.suit_) {}

    friend std::istream& operator >>(std::istream& in, card_t& card)
    {
        char rank(0), suit(0);
        in >> rank >> suit;
        card.rank_ = static_cast<rank_t>(rank);
        card.suit_ = static_cast<suit_t>(suit);
        return in;
    }

    friend std::ostream& operator <<(std::ostream& out, const card_t& card)
    {
        out << static_cast<char>(card.rank_) << static_cast<char>(card.suit_);
        return out;
    }
};

namespace poker {

enum class combination_t : uint8_t {
    HighCard = 0, OnePair, TwoPairs, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush
};

}  // namespace poker
