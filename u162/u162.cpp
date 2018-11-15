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
#include <stack>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U162 instance;
    instance();
}

namespace {
typedef std::pair<char, char> card;

class desk {
    std::vector<std::stack<card>> hands_;
    std::stack<card> table_;
    size_t player_;
    bool valid_;
    bool over_;
public:

    desk() :
        hands_(), table_(), player_(0), valid_(true), over_(false)
    {
        hands_.resize(2);
    }

    friend std::istream& operator >>(std::istream& in, desk& eng);
    friend std::ostream& operator <<(std::ostream& out, const desk& eng);

    operator bool() const
    {
        return valid_;
    }

    desk& operator ()();

private:
    void playToCover(size_t steps);
    void coverFace();
    bool step();
    void changeTurn();

    void dumpRound();

    void reinit();

    friend void deal(std::string& line, desk& eng, std::istream& in);
    void incorporateTable();
};

void desk::reinit()
{
    std::for_each(hands_.begin(), hands_.end(), [](std::stack<card>& hand) {
        while (!hand.empty()) {
            hand.pop();
        }
    });

    while (!table_.empty()) {
        table_.pop();
    }

    over_ = false;
}

void deal(std::string& line, desk& eng, std::istream& in)
{
    eng.reinit();

    for (int i = 0, cardNo = 0; i < 4; i++) {
        std::stringstream ss(line);
        std::istream_iterator<std::string> iss(ss);
        std::for_each(iss, std::istream_iterator<std::string>(), [&](const std::string & word) {
            eng.hands_[(cardNo++) % 2].push(card(word[0], word[1]));
        });

        if (i < 3) {
            std::getline(in, line);
        }
    }
}

std::istream& operator >>(std::istream& in, desk& eng)
{
    std::string line;
    std::getline(in, line);
    eng.valid_ = in && !line.empty() && line != "#";

    if (eng.valid_) {
        deal(line, eng, in);
    }

    return in;
}

std::ostream& operator <<(std::ostream& out, const desk& eng)
{
    out << ((eng.player_) % 2) + 1;
    out.setf(std::ios::right);
    out << std::setw(3) << eng.hands_[(eng.player_ + 1) % 2].size();
    return out;
}

void desk::incorporateTable()
{
    changeTurn();
    std::stack<card> temp;

    while (!hands_[player_].empty()) {
        temp.push(hands_[player_].top());
        hands_[player_].pop();
    }

    while (!table_.empty()) {
        hands_[player_].push(table_.top());
        table_.pop();
    }

    while (!temp.empty()) {
        hands_[player_].push(temp.top());
        temp.pop();
    }
}

void desk::playToCover(size_t steps)
{
    while (steps--) {
        if (step()) {
            switch (table_.top().second) {
            case 'J':
            case 'Q':
            case 'K':
            case 'A':
                steps = 0;
                changeTurn();
                coverFace();
                break;

            default:
                break;
            }

        } else {
            over_ = true;
            break;
        }
    }

    if (!over_ && !table_.empty()) {
        incorporateTable();
        dumpRound();
    }
}

void desk::changeTurn()
{
    player_ = (player_ + 1) & 0x01;
}

std::ostream& operator << (std::ostream& out, std::stack<card> cards)
{
    out << cards.size() << ":[";

    while (!cards.empty()) {
        out << " " << cards.top().first << cards.top().second << " ";
        cards.pop();
    }

    out << "]";
    return out;
}

void desk::dumpRound()
{
//    std::clog << player_ << ": " << table_ << std::endl;
//    std::clog << "\t" << hands_[0] << std::endl;
//    std::clog << "\t" << hands_[1] << std::endl;
}

bool desk::step()
{
    if (!hands_[player_].empty()) {
        table_.push(hands_[player_].top());
        hands_[player_].pop();
        dumpRound();
        return true;
    }

    return false;
}

desk& desk::operator ()()
{
    player_ = 0;

    while (step()) {
        changeTurn();
        coverFace();
    }

    return *this;
}

void desk::coverFace()
{
    switch (table_.top().second) {
    case 'J':
        playToCover(1);
        break;

    case 'Q':
        playToCover(2);
        break;

    case 'K':
        playToCover(3);
        break;

    case 'A':
        playToCover(4);
        break;

    default:
        break;
    }
}
} // namespace

void U162::operator()() const
{
    desk eng;

    while ((std::cin >> eng) && eng) {
        std::cout << eng() << std::endl;
    }
}
