#include "u401.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <unordered_map>

namespace {
class palindrome {
    std::unordered_map<char, char> valid_chars_;
public:
    enum class kind :int {
        NotAtAll = 0, Palindrome = 1, Mirror = 2, MirrorPalindrome = 3
    };

    palindrome () : valid_chars_({
        {'A','A'},{'M','M'},{'Y','Y'},
        /*{'B',0},{'N', 0},*/{'Z','5'},
        /*{'C',0},*/{'O','O'},{'1','1'},
        /*{'D',0},{'P',0},*/{'2','S'},
        {'E','3'},/*{'Q',0},*/{'3','E'},
        /*{'F',0},{'R',0},{'4',0},
        {'G',0},*/{'S','2'},{'5','Z'},
        {'H','H'},{'T','T'},/*{'6',0},*/
        {'I','I'},{'U','U'},/*{'7',0},*/
        {'J','L'},{'V','V'},{'8','8'},
        /*{'K',0},*/{'W','W'},/*{'9',0},*/
        {'L','J'},{'X','X'}
    }) {}

    kind check(const std::string& source);

};

palindrome::kind palindrome::check(const std::string& source)
{
    int result(int(kind::NotAtAll));
    std::string working (source);
    std::reverse(working.begin(), working.end());
    if (working == source) {
        result |= int(kind::Palindrome);
    }
    std::transform(working.begin(), working.end(), working.begin(), [&](const char ch) -> char { return valid_chars_[ch]; });
    if (working == source) {
        result |= int(kind::Mirror);
    }
    return kind(result);
}

}

void U401::operator()()
{
    std::string line;
    palindrome p;
    while (std::getline(std::cin, line)) {
        std::cout << line;
        switch (p.check(line)) {
        case palindrome::kind::NotAtAll:
            std::cout << " -- is not a palindrome." << std::endl;
            break;
        case palindrome::kind::Palindrome:
            std::cout << " -- is a regular palindrome." << std::endl;
            break;
        case palindrome::kind::Mirror:
            std::cout << " -- is a mirrored string." << std::endl;
            break;
        case palindrome::kind::MirrorPalindrome:
            std::cout << " -- is a mirrored palindrome." << std::endl;
            break;
        }
        std::cout << std::endl;
    }
}
