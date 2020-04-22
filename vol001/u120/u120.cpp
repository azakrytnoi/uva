#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u120.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U120 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution();

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        solution& operator() ();

    private:
        std::vector<uint16_t> pancakes_;
        std::list<size_t> moves_;

        bool isSorted() const;
        bool moveLargestToFirst(size_t pos);
    };

    solution::solution() : pancakes_(), moves_()
    {
        pancakes_.reserve(30);
    }

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.pancakes_.clear();
        std::copy(std::istream_iterator<uint16_t>(in), std::istream_iterator<uint16_t>(), std::back_inserter(sol.pancakes_));
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        std::ostream_iterator<size_t> iout(out, " ");
        std::copy(sol.moves_.begin(), sol.moves_.end(), iout);
        out << "0";
        return out;
    }

    solution& solution::operator()()
    {
        moves_.clear();

        for (size_t i = pancakes_.size(); not isSorted(); i--)
        {
            if (moveLargestToFirst(i))
            {
                moves_.push_back(pancakes_.size() - i + 1);
                std::reverse(pancakes_.begin(), pancakes_.begin() + i);
            }
        }

        return *this;
    }

    bool solution::isSorted() const
    {
        for (size_t i = 0; i < pancakes_.size() - 1; i++)
        {
            if (pancakes_[i] > pancakes_[i + 1])
            {
                return false;
            }
        }

        return true;
    }
    bool solution::moveLargestToFirst(size_t pos)
    {
        auto max_elt = std::max_element(pancakes_.begin(), pancakes_.begin() + pos);

        if (max_elt == pancakes_.begin() + pos - 1)
        {
            return false;
        }
        else if (max_elt != pancakes_.begin())
        {

            std::reverse(pancakes_.begin(), max_elt + 1);
            size_t largest = max_elt - pancakes_.begin();
            moves_.push_back(pancakes_.size() - largest);
        }

        return true;
    }
}

void U120::operator()() const
{
    solution sol;
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::cout << line << std::endl;
        std::stringstream in(line);
        in >> sol;
        std::cout << sol() << std::endl;
    }
}
