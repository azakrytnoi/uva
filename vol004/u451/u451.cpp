
#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u451.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U450 instance;
    instance();
}

namespace {

    enum class suit_t : char {
        Spades = 'S', Hearts = 'H', Diamonds = 'D', Clubs = 'C'
    };

    enum class rank_t : char {
        A = 'A', _2 = '2', _3 = '3', _4 = '4', _5 = '5', _6 = '6', _7 = '7', _8 = '8', _9 = '9', X = 'X', J = 'J', Q = 'Q', K = 'K'
    };

    enum class combination_t : uint8_t {
        Nothing = 0, OnePair, TwoPairs, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraitFlush
    };

    class card_t {
    public:
        rank_t rank_;
        suit_t suit_;

        card_t() : rank_(), suit_() {}
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

    class solution_t {
    public:
        solution_t() : n_(std::numeric_limits<size_t>::max()), desk_(), result_( {
            {combination_t::Nothing, 0},
            {combination_t::OnePair, 0},
            {combination_t::TwoPairs, 0},
            {combination_t::ThreeOfAKind, 0},
            {combination_t::Straight, 0},
            {combination_t::Flush, 0},
            {combination_t::FullHouse, 0},
            {combination_t::FourOfAKind, 0},
            {combination_t::StraitFlush, 0},
        }) {}

        operator bool()
        {
            return n_-- > 0;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t n_;
        std::array<std::array<card_t, 5>, 5> desk_;
        std::map<combination_t, size_t> result_;

        typedef std::array<card_t, 5>::iterator hand_iterator_t;
        combination_t hand_value(hand_iterator_t hbegin, hand_iterator_t hend) const;
    };

    solution_t& solution_t::operator()()
    {
        for (auto& res : result_) {
            res.second = 0;
        }

        for (size_t i = 0; i < 5; ++i) {
            std::array<card_t, 5> col;

            for (size_t j = 0; j < 5; ++j) {
                col[j] = desk_[j][i];
            }

            result_[hand_value(desk_[i].begin(), desk_[i].end())]++;
            result_[hand_value(col.begin(), col.end())]++;
        }

        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        if (sol.n_ == std::numeric_limits<size_t>::max()) {
            in >> sol.n_;
        }

        if (in) {
            for (size_t i = 0; i < 5; ++i) {
                for (size_t j = 0; j < 5; ++j) {
                    in >> sol.desk_[i][j];
                }
            }
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        auto begin = sol.result_.begin();
        out << begin->second;
        ++begin;
        std::for_each(begin, sol.result_.end(), [&](const std::pair<combination_t, size_t>& freq) {
            out << ", " << freq.second;
        });
        out << std::endl;
        return out;
    }

    combination_t solution_t::hand_value(hand_iterator_t hbegin, hand_iterator_t hend) const
    {
//        std::ostream_iterator<card_t> clog(std::clog, " ");
//        std::copy(hbegin, hend, clog);
        static const std::vector<rank_t> rorder({rank_t::A, rank_t::_2, rank_t::_3, rank_t::_4, rank_t::_5, rank_t::_6, rank_t::_7, rank_t::_8, rank_t::_9, rank_t::X, rank_t::J, rank_t::Q, rank_t::K});
        struct less_rank {
            bool operator()(const rank_t lhs, const rank_t rhs)
            {
                auto lhsi = std::find(rorder.begin(), rorder.end(), lhs);
                auto rhsi = std::find(rorder.begin(), rorder.end(), rhs);
                return std::distance(lhsi, rhsi) > 0;
            }
        };
        std::map<rank_t, size_t, less_rank> ranks;
        std::map<suit_t, size_t> suits;
        std::for_each(hbegin, hend, [&](const card_t& card) {
            ranks[card.rank_]++;
            suits[card.suit_]++;
        });
        std::vector<size_t> rfreq(ranks.size());
        std::transform(ranks.begin(), ranks.end(), rfreq.begin(), [](const std::pair<rank_t, size_t>& r) {
            return r.second;
        });
        std::sort(rfreq.begin(), rfreq.end(), [](size_t lhs, size_t rhs) {
            return lhs > rhs;
        });
        std::vector<size_t> sfreq(suits.size());
        std::transform(suits.begin(), suits.end(), sfreq.begin(), [](const std::pair<suit_t, size_t>& s) {
            return s.second;
        });
        std::sort(sfreq.begin(), sfreq.end(), [](size_t lhs, size_t rhs) {
            return lhs > rhs;
        });

        switch (rfreq.front()) {
        case 2:
//            std::clog << " => " << (sfreq.front() == 5 ? "Flush" : rfreq[1] == 2 ? "Two Pairs" : "One Pair") << std::endl;
            return sfreq.front() == 5 ? combination_t::Flush : rfreq[1] == 2 ? combination_t::TwoPairs : combination_t::OnePair;

        case 3:
//            std::clog << " => " << (sfreq.front() == 5 ? "Flush" : rfreq[1] == 2 ? "Full House" : "Three of a Kind") << std::endl;
            return sfreq.front() == 5 ? combination_t::Flush : rfreq[1] == 2 ? combination_t::FullHouse : combination_t::ThreeOfAKind;

        case 4:
//            std::clog << " => " << "Four of a Kind" << std::endl;
            return combination_t::FourOfAKind;

        default: {
            auto r = ranks.begin();
            auto prev_rank = std::find(rorder.begin(), rorder.end(), r->first);
            ++r;
            bool strait = true;

            for (; strait && r != ranks.end(); ++r) {
                auto cur_rank = std::find(rorder.begin(), rorder.end(), r->first);
                auto distance = std::distance(prev_rank, cur_rank);
                strait = distance == 1 || distance == 9;
                prev_rank = cur_rank;
            }

            if (strait) {
//                std::clog << " => " << (sfreq.front() == 5 ? "Strait Flush" : "Strait") << std::endl;
                return sfreq.front() == 5 ? combination_t::StraitFlush : combination_t::Straight;
            }
        }
        break;
        }

//        std::clog << " => " << (sfreq.front() == 5 ? "Flush" : "Nothing") << std::endl;
        return sfreq.front() == 5 ? combination_t::Flush : combination_t::Nothing;
    }

}

void U450::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
