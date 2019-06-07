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

#include "card.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U451 instance;
    instance();
}

namespace {

    namespace poker {

        typedef ::poker::combination_t combination_t;

        typedef X10::rank_t rank_t;
        typedef std::array<card_t<rank_t>, 5> hand_t;
        typedef hand_t::iterator hand_iterator_t;

        combination_t hand_value(hand_iterator_t hbegin, hand_iterator_t hend)
        {
            static const std::vector<rank_t> rorder({rank_t::A, rank_t::_2, rank_t::_3, rank_t::_4, rank_t::_5, rank_t::_6, rank_t::_7, rank_t::_8, rank_t::_9, rank_t::X, rank_t::J, rank_t::Q, rank_t::K});
            struct less_rank
            {
                bool operator()(const rank_t lhs, const rank_t rhs) const
                {
                    auto lhsi = std::find(rorder.begin(), rorder.end(), lhs);
                    auto rhsi = std::find(rorder.begin(), rorder.end(), rhs);
                    return std::distance(lhsi, rhsi) > 0;
                }
            };
            std::map<rank_t, size_t, less_rank> ranks;
            std::map<suit_t, size_t> suits;
            std::for_each(hbegin, hend, [&](const card_t<>& card)
            {
                ranks[card.rank_]++;
                suits[card.suit_]++;
            });
            std::vector<size_t> rfreq(ranks.size());
            std::transform(ranks.begin(), ranks.end(), rfreq.begin(), [](const std::pair<rank_t, size_t>& r)
            {
                return r.second;
            });
            std::sort(rfreq.begin(), rfreq.end(), [](size_t lhs, size_t rhs)
            {
                return lhs > rhs;
            });
            std::vector<size_t> sfreq(suits.size());
            std::transform(suits.begin(), suits.end(), sfreq.begin(), [](const std::pair<suit_t, size_t>& s)
            {
                return s.second;
            });
            std::sort(sfreq.begin(), sfreq.end(), [](size_t lhs, size_t rhs)
            {
                return lhs > rhs;
            });

            switch (rfreq.front())
            {
            case 2:
                return sfreq.front() == 5 ? combination_t::Flush : rfreq[1] == 2 ? combination_t::TwoPairs : combination_t::OnePair;

            case 3:
                return sfreq.front() == 5 ? combination_t::Flush : rfreq[1] == 2 ? combination_t::FullHouse : combination_t::ThreeOfAKind;

            case 4:
                return combination_t::FourOfAKind;

            default:
            {
                auto r = ranks.begin();
                auto prev_rank = std::find(rorder.begin(), rorder.end(), r->first);
                ++r;
                bool strait = true;

                for (; strait && r != ranks.end(); ++r)
                {
                    auto cur_rank = std::find(rorder.begin(), rorder.end(), r->first);
                    auto distance = std::distance(prev_rank, cur_rank);
                    strait = distance == 1 || distance == 9;
                    prev_rank = cur_rank;
                }

                if (strait)
                {
                    return sfreq.front() == 5 ? combination_t::StraightFlush : combination_t::Straight;
                }
            }
            break;
            }

            return sfreq.front() == 5 ? combination_t::Flush : combination_t::HighCard;
        }

    }  // namespace poker

    class solution_t {
        typedef poker::combination_t combination_t;

    public:
        solution_t() : n_(std::numeric_limits<size_t>::max()), desk_(), result_(
        {
            {combination_t::HighCard, 0},
            {combination_t::OnePair, 0},
            {combination_t::TwoPairs, 0},
            {combination_t::ThreeOfAKind, 0},
            {combination_t::Straight, 0},
            {combination_t::Flush, 0},
            {combination_t::FullHouse, 0},
            {combination_t::FourOfAKind, 0},
            {combination_t::StraightFlush, 0},
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
        std::array<poker::hand_t, 5> desk_;
        std::map<combination_t, size_t> result_;

    };

    solution_t& solution_t::operator()()
    {
        for (auto& res : result_)
        {
            res.second = 0;
        }

        for (size_t i = 0; i < 5; ++i)
        {
            poker::hand_t col;

            for (size_t j = 0; j < 5; ++j)
            {
                col[j] = desk_[j][i];
            }

            result_[poker::hand_value(desk_[i].begin(), desk_[i].end())]++;
            result_[poker::hand_value(col.begin(), col.end())]++;
        }

        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        if (sol.n_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.n_;
        }

        if (in)
        {
            for (size_t i = 0; i < 5; ++i)
            {
                for (size_t j = 0; j < 5; ++j)
                {
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
        std::for_each(begin, sol.result_.end(), [&](const std::pair<poker::combination_t, size_t>& freq)
        {
            out << ", " << freq.second;
        });
        out << std::endl;
        return out;
    }

}

void U451::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
