#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u554.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <set>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U554 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : dictionary_(), crypted_(), plain_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        std::set<std::string> dictionary_;
        std::string crypted_;
        std::string plain_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::string line;

        while (std::getline(in, line) && line != "#")
        {
            sol.dictionary_.insert(line);
        }

        std::getline(in, sol.crypted_);
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        std::stringstream parser(sol.plain_);
        std::istream_iterator<std::string> tokenizer(parser);
        std::ostream_iterator<std::string> oit(out, "");
        std::transform(tokenizer, std::istream_iterator<std::string>(), oit, [&](auto & word)
        {
            static size_t line_length(0);

            if (line_length + word.length() >= 60)
            {
                out << std::endl;
                line_length = 0;
            }

            if (line_length > 0)
            {
                out << ' ';
            }

            line_length += word.length() + 1;

            return word;
        });
        return out;
    }

    solution& solution::operator()()
    {
        std::string result;
        result.resize(crypted_.size());
        size_t maxcnt(0);

        for (int16_t key = 1; key < 27; key++)
        {
            std::transform(crypted_.begin(), crypted_.end(), result.begin(), [&](char ch)
            {
                char shift = (ch == ' ') ? 0 : ch - 'A' + 1;
                shift = (shift - key + 27) % 27;
                return shift == 0 ? ' ' : shift + 'A' - 1;
            });
            std::stringstream temp(result);
            std::istream_iterator<std::string> token(temp);
            size_t cnt = std::count_if(token, std::istream_iterator<std::string>(), [&](auto & word)
            {
                return dictionary_.find(word) != dictionary_.end();
            });

            if (cnt > maxcnt)
            {
                plain_.assign(result.begin(), result.end());
                maxcnt = cnt;
            }
        }

        return *this;
    }

}

void U554::operator()() const
{
    solution sol;
    std::cin >> sol;
    std::cout << sol() << std::endl;
}
