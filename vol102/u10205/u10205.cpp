#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10205.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

#include "card.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10205 instance;
    instance();
}

namespace {

    typedef T10::rank_t rank;
    typedef suit_t suit;

    std::ostream& operator << (std::ostream& out, const suit& s)
    {
        switch (s)
        {
        case suit::Clubs:
            out << "Clubs";
            break;

        case suit::Diamonds:
            out << "Diamonds";
            break;

        case suit::Hearts:
            out << "Hearts";
            break;

        case suit::Spades:
            out << "Spades";
            break;

        default:
            break;
        }

        return out;
    }

    std::ostream& operator << (std::ostream& out, const rank& r)
    {
        switch (r)
        {
        case rank::A:
            out << "Ace";
            break;

        case rank::K:
            out << "King";
            break;

        case rank::Q:
            out << "Queen";
            break;

        case rank::J:
            out << "Jack";
            break;

        case rank::T:
            out << "10";
            break;

        default:
            out << char(r);
            break;
        }

        return out;
    }

    class card : public card_t<T10::rank_t> {
    public:
        card(const rank& r, const suit& s) : card_t<T10::rank_t>(r, s) {}

        friend bool operator < (const card& c1, const card& c2)
        {
            if (c1.suit_ == c2.suit_)
            {
                return c1.rank_ < c2.rank_;
            }

            return c1.suit_ < c2.suit_;
        }

        friend std::ostream& operator << (std::ostream& out, const card& c)
        {
            out << c.rank_ << " of " << c.suit_;
            return out;
        }
    };


    class solution {
        std::vector<card> desk_;
    public:
        solution();

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    private:
    };

    solution::solution() : desk_()
    {
        suit suits[] = { suit::Clubs, suit::Diamonds, suit::Hearts, suit::Spades };
        rank ranks[] = { rank::_2, rank::_3, rank::_4, rank::_5, rank::_6, rank::_7, rank::_8, rank::_9, rank::T, rank::J, rank::Q, rank::K, rank::A };
        desk_.reserve(52);
        std::for_each(suits, suits + 4, [&](const suit & s)
        {
            std::for_each(ranks, ranks + 13, [&](const rank & r)
            {
                desk_.push_back(card(r, s));
            });
        });
    }

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        uint32_t n;
        in >> n;
        std::vector<std::vector<uint16_t>> shuffles;
        shuffles.reserve(n);

        while (n--)
        {
            std::vector<uint16_t> temp;
            temp.reserve(52);
            std::istream_iterator<uint16_t> iit(in);
            std::copy_n(iit, 52, std::back_inserter(temp));
            shuffles.push_back(temp);
        }

        std::string line;
        std::getline(in, line);

        while (std::getline(in, line) && not line.empty())
        {
            std::vector<card> temp;
            temp.reserve(52);
            std::stringstream ss (line);
            ss >> n;
            std::for_each(shuffles[n - 1].begin(), shuffles[n - 1].end(), [&](uint16_t idx)
            {
                temp.push_back(sol.desk_[idx - 1]);
            });
            sol.desk_.swap(temp);
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        std::ostream_iterator<card> oit(out, "\n");
        std::copy(sol.desk_.begin(), sol.desk_.end(), oit);
        return out;
    }

}

void U10205::operator()() const
{
    uint32_t N;
    std::cin >> N;

    while (N--)
    {
        solution sol;
        std::cin >> sol;
        std::cout << sol << std::endl;
    }
}
