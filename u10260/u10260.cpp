#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10260.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <unordered_map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10260 instance;
    instance();
}

namespace {

class solution {
    static std::unordered_map<char, char> soundex_;
    std::string original_;
public:
    solution() : original_() {}

    friend std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::getline(in, sol.original_);
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, solution& sol)
    {
        char last(0);
        std::for_each(sol.original_.begin(), sol.original_.end(), [&](auto ch) {
            char soundex (sol.soundex(ch));

            if (soundex > 0 && soundex != last) {
                out << soundex;
            }

            last = soundex;
        });
        return out;
    }

private:
    static char soundex(char ch);
};

char solution::soundex(char ch)
{
    switch (ch) {
    case 'B':
    case 'F':
    case 'P':
    case 'V':
        return '1';

    case 'C':
    case 'G':
    case 'J':
    case 'K':
    case 'Q':
    case 'S':
    case 'X':
    case 'Z':
        return '2';

    case 'D':
    case 'T':
        return '3';

    case 'L':
        return '4';

    case 'M':
    case 'N':
        return '5';

    case 'R':
        return '6';

    default:
        return 0;
    }
}

std::unordered_map<char, char> solution::soundex_ = {
    {'B', '1' }, {'F', '1' }, {'P', '1' }, {'V', '1' },
    {'C', '2' }, {'G', '2' }, {'J', '2' }, {'K', '2' }, {'Q', '2' }, {'S', '2' }, {'X', '2' }, {'Z', '2' },
    {'D', '3' }, {'T', '3' },
    {'L', '4' },
    {'M', '5' }, {'N', '5' },
    {'R', '6' }
};

}  // namespace

void U10260::operator()() const
{
    solution sol;

    while (std::cin >> sol) {
        std::cout << sol << std::endl;
    }
}
