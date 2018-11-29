#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10029.h"

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
    U10029 instance;
    instance();
}

namespace {

    class solution {
        std::vector<std::string> words_;
        int32_t maxLen_;
    public:
        solution() : words_(), maxLen_()
        {
            words_.reserve(25000);
        }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::istream_iterator<std::string> iin(in);
        std::copy(iin, std::istream_iterator<std::string>(), std::back_inserter(sol.words_));
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.maxLen_;
        return out;
    }

    solution& solution::operator()()
    {
        std::unordered_map <std::string, int32_t> wordCache;
        std::for_each(words_.begin(), words_.end(), [&](std::string & word) {
            int32_t len(1);

            for (size_t i = 0; i <= word.size(); ++i) {
                for (char c = 'a'; c <= 'z'; ++c) {
                    std::string w1(word);
                    w1.insert(i, 1, c);

                    if (w1 > word) {
                        break;
                    }

                    if (wordCache.count(w1)) {
                        len = std::max(len, wordCache[w1] + 1);
                    }
                }
            }

            for (size_t i = 0; i < word.size(); ++i) {
                std::string w1(word);
                w1.erase(i, 1);

                if (wordCache.count(w1)) {
                    len = std::max(len, wordCache[w1] + 1);
                }
            }

            for (size_t i = 0; i < word.size(); ++i) {
                for (char c = 'a'; c <= 'z' && c != word[i]; ++c) {
                    std::string w1(word);
                    w1[i] = c;

                    if (w1 > word) {
                        break;
                    }

                    if (wordCache.count(w1)) {
                        len = std::max(len, wordCache[w1] + 1);
                    }
                }
            }

            wordCache[word] = len;
            maxLen_ = std::max(maxLen_, len);
        });
        return *this;
    }

}

void U10029::operator()() const
{
    solution sol;
    std::cin >> sol;
    std::cout << sol() << std::endl;
}
