#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u492.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>
#include <cctype>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U492 instance;
	instance();
}

namespace  {

class piggler {
public:
    static void piggle(const std::string& word);
};

void piggler::piggle(const std::string& word) {
    switch (word[0]) {
    case 'a':
    case 'A':
    case 'e':
    case 'E':
    case 'i':
    case 'I':
    case 'o':
    case 'O':
    case 'u':
    case 'U':
        std::cout << word;
        break;
    default:
        std::cout << word.substr(1) << word[0];
        break;
    }
    std::cout << "ay";
}

}  // namespace

void U492::operator()()
{
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::string word;
        for (auto it = line.begin(); it != line.end(); ++it) {
            if (std::isalpha(*it)) {
                word += (*it);
            } else {
                if (!word.empty()) {
                    piggler::piggle(word);
                    word.clear();
                }
                std::cout << (*it);
            }
        }
        if (!word.empty()) {
            piggler::piggle(word);
        }
        std::cout << std::endl;
    }
}
