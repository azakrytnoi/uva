#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u454.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning (disable : 4503)
#endif // _WIN32

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <cmath>
#include <sstream>
#include <string>
#include <map>
#include <cctype>
#include <list>

U454::U454()
{
}

U454::~U454()
{
}

void U454::operator()()
{
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    while (N--)
    {
        std::map<std::string, std::map<std::string, std::list<std::string>>> anagrams;
        size_t lines(0);
        while (std::getline(std::cin, line) && !line.empty())
        {
            lines++;
            std::string akey;
            std::copy_if(line.begin(), line.end(), std::back_inserter(akey), [](auto ch) { return ch != ' '; });
            std::transform(akey.begin(), akey.end(), akey.begin(), [](auto ch) { return std::tolower(ch); });
            std::sort(akey.begin(), akey.end());
            auto it = anagrams.find(akey);
            if (it == anagrams.end())
            {
                anagrams[akey][line].clear();
            }
            else
            {
                std::for_each(it->second.begin(), it->second.end(), [&](const std::pair<std::string, std::list<std::string>>& item) {
                    it->second[item.first].push_back(line);
                });
                it->second[line].clear();
            }
        }
        std::vector<std::pair<std::string, std::string>> result;
        result.reserve(lines);
        std::for_each(anagrams.begin(), anagrams.end(), [&](const std::pair<std::string, std::map<std::string, std::list<std::string>>>& item)
        {
            std::for_each(item.second.begin(), item.second.end(), [&](const std::pair<std::string, std::list<std::string>>& pairs)
            {
                std::for_each(pairs.second.begin(), pairs.second.end(), [&](const std::string& anagram) {
                    if (pairs.first < anagram)
                    {
                        result.push_back(std::make_pair(pairs.first, anagram));
                    }
                    else
                    {
                        result.push_back(std::make_pair(anagram, pairs.first));
                    }
                });
            });
        });
        std::sort(result.begin(), result.end(), [](auto p1, auto p2) -> bool
        {
            if (p1.first == p2.first)
            {
                return p1.second < p2.second;
            }
            return p1.first < p2.first;
        });
        std::for_each(result.begin(), result.end(), [](auto p)
        {
            std::cout << p.first << " = " << p.second << std::endl;
        });
        std::cout << std::endl;
    }
}

#ifdef _WIN32
#pragma warning (pop)
#endif
