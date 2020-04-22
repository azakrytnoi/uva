#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u162.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
// #include <stack>
#include <sstream>
#include <deque>

#include "card.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U162 instance;
    instance();
}

namespace {

    class desk_t {
        std::vector<std::deque<cardT10_t>> hands_;
        std::deque<cardT10_t> table_;
        size_t player_;
        bool valid_;
        bool over_;

    public:
        desk_t() :
            hands_(), table_(), player_(0), valid_(true), over_(false)
        {
            hands_.resize(2);
        }

        friend std::istream& operator >>(std::istream& in, desk_t& eng);
        friend std::ostream& operator <<(std::ostream& out, const desk_t& eng);

        operator bool() const
        {
            return valid_;
        }

        desk_t& operator ()();

    private:
        void playToCover(size_t steps);
        void coverFace();
        bool step();
        void changeTurn();

        void dumpRound();

        void reinit();

        friend void deal(std::string& line, desk_t& eng, std::istream& in);
        void incorporateTable();
    };

    void desk_t::reinit()
    {
        for (auto& hand : hands_)
        {
            hand.clear();
        }

        table_.clear();

        over_ = false;
    }

    void deal(std::string& line, desk_t& eng, std::istream& in)
    {
        eng.reinit();

        for (int i = 0, cardNo = 0; i < 4; i++)
        {
            std::stringstream ss(line);
            std::istream_iterator<std::string> iss(ss);
            std::for_each(iss, std::istream_iterator<std::string>(), [&](const std::string & word)
            {
                eng.hands_[(cardNo++) % 2].push_front(cardT10_t(word[1], word[0]));
            });

            if (i < 3)
            {
                std::getline(in, line);
            }
        }
    }

    std::istream& operator >>(std::istream& in, desk_t& eng)
    {
        std::string line;
        std::getline(in, line);
        eng.valid_ = in && !line.empty() && line != "#";

        if (eng.valid_)
        {
            deal(line, eng, in);
        }

        return in;
    }

    std::ostream& operator <<(std::ostream& out, const desk_t& eng)
    {
        out << ((eng.player_) % 2) + 1;
        out.setf(std::ios::right);
        out << std::setw(3) << eng.hands_[(eng.player_ + 1) % 2].size();
        return out;
    }

    void desk_t::incorporateTable()
    {
        changeTurn();

        while (not table_.empty()) {
            hands_[player_].push_back(table_.back());
            table_.pop_back();
        }
    }

    void desk_t::playToCover(size_t steps)
    {
        while (steps--)
        {
            if (step())
            {
                switch (table_.front().rank_)
                {
                case cardT10_t::rank_t::J:
                case cardT10_t::rank_t::Q:
                case cardT10_t::rank_t::K:
                case cardT10_t::rank_t::A:
                    steps = 0;
                    changeTurn();
                    coverFace();
                    break;

                default:
                    break;
                }
            }
            else
            {
                over_ = true;
                break;
            }
        }

        if (not over_ && not table_.empty())
        {
            incorporateTable();
            dumpRound();
        }
    }

    void desk_t::changeTurn()
    {
        player_ = (player_ + 1) & 0x01;
    }

    std::ostream& operator << (std::ostream& out, std::deque<cardT10_t> cards)
    {
        out << cards.size() << ":[";

        while (!cards.empty())
        {
            out << " " << static_cast<char>(cards.front().suit_) << static_cast<char>(cards.front().rank_) << " ";
            cards.pop_front();
        }

        out << "]";
        return out;
    }

    void desk_t::dumpRound()
    {
        std::clog << player_ << ": " << table_ << std::endl;
        std::clog << "\t" << hands_[0] << std::endl;
        std::clog << "\t" << hands_[1] << std::endl;
    }

    bool desk_t::step()
    {
        if (not hands_[player_].empty())
        {
            table_.push_front(hands_[player_].front());
            hands_[player_].pop_front();
            dumpRound();
            return true;
        }

        return false;
    }

    desk_t& desk_t::operator ()()
    {
        player_ = 0;

        while (step())
        {
            changeTurn();
            coverFace();
        }

        return *this;
    }

    void desk_t::coverFace()
    {
        switch (table_.front().rank_)
        {
        case cardT10_t::rank_t::J:
            playToCover(1);
            break;

        case cardT10_t::rank_t::Q:
            playToCover(2);
            break;

        case cardT10_t::rank_t::K:
            playToCover(3);
            break;

        case cardT10_t::rank_t::A:
            playToCover(4);
            break;

        default:
            break;
        }
    }
} // namespace

void U162::operator()() const
{
    desk_t eng;

    while ((std::cin >> eng) && eng)
    {
        std::cout << eng() << std::endl;
    }
}
