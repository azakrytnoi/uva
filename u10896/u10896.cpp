#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10896.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10896 instance;
    instance();
}

namespace
{

class decoder
{
    std::map<size_t, std::list<std::string>> words_;

public:
    explicit decoder(const std::string& sentence) :
        words_()
    {
        std::stringstream ss(sentence);
        std::istream_iterator<std::string> sin(ss);
        std::for_each(sin, std::istream_iterator<std::string>(), [&](const std::string& word) {
            words_[word.length()].push_back(word);
        });
    }

    std::string decode(const std::string& word)
    {
        std::stringstream out;
        std::list<std::string>& candidates = words_[word.length()];
        for (auto it = candidates.begin(); it != candidates.end(); ++it) {
            std::string::const_iterator wit = word.begin();
            char candidate = distance((*it)[0], *wit);
            if (std::accumulate((*it).begin(), (*it).end(), true, [&](bool current, char ch) -> bool {
            return current &= distance(ch, *(wit++)) == candidate;
            })) {
                out << char('a' + candidate);
            }
        }
        std::string result(out.str());
        std::sort(result.begin(), result.end());
        return result;
    }

private:
    char distance (char a, char b)
    {
        char res = a - b;
        if (res < 0) {
            res += ('z' - 'a') + 1;
        }
        return res;
    }
};

}  // namespace

void U10896::operator()()
{
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    while (N--) {
        std::getline(std::cin, line);
        decoder d(line);
        std::getline(std::cin, line);
        std::cout << d.decode(line) << std::endl;
    }
}
