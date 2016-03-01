#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u156.h"

#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <iterator>

U156::U156()
{
}

U156::~U156()
{
}

extern "C" { 	UVA_API_EXPORT void __cdecl invoke(); } void __cdecl invoke() { 	U156 instance; 	instance(); }
void U156::operator()()
{
    std::string word;
    std::map<std::string, std::pair<std::string, int>> dictionary;
    while (std::cin >> word && word[0] != '#') {
        std::string key(word);
        std::transform(key.begin(), key.end(), key.begin(), [](auto ch) {
            return std::tolower(ch);
        });
        std::sort(key.begin(), key.end());
        if (dictionary[key].second++ == 0) {
            dictionary[key].first = word;
        }
    }
    std::vector <std::pair<std::string, std::pair<std::string, int>>> ordinals;
    ordinals.reserve(dictionary.size());
    std::copy_if(dictionary.begin(), dictionary.end(), std::back_inserter(ordinals), [](auto item) -> bool { return item.second.second == 1; });
    std::sort(ordinals.begin(), ordinals.end(), [](auto a1, auto a2) -> bool { return a1.second.first < a2.second.first; });
    std::ostream_iterator<std::string> out(std::cout, "\n");
    std::transform(ordinals.begin(), ordinals.end(), out, [](auto item) -> std::string { return item.second.first; });
}
