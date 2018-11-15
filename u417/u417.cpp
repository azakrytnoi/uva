#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u417.h"

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <queue>

U417::U417()
{}

namespace {
class solver {
    std::map<std::string, int> valid_;
public:
    solver() : valid_()
    {
        std::queue<std::string> work;

        for (char ch = 'a'; ch <= 'z'; ch++) {
            work.push(std::string(1, ch));
        }

        std::string word;
        int cnt(1);

        while (!work.empty()) {
            word = work.front();
            work.pop();
            valid_[word] = cnt++;

            if (word.length() == 5) {
                continue;
            }

            for (char ch = *(--word.end()) + 1; ch <= 'z'; ch++) {
                work.push(word + ch);
            }
        }
    }

    int check(std::string& word)
    {
        auto it = valid_.find(word);

        if (it != valid_.end()) {
            return it->second;
        }

        return 0;
    }
};
}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U417 instance;
    instance();
}
void U417::operator()() const
{
    solver s;
    std::string word;

    while (std::cin >> word) {
        std::cout << s.check(word) << std::endl;
    }
}
