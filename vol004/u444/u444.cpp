#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u444.h"

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
    U444 instance;
    instance();
}

namespace {

    class coder {
    public:

        static std::string encode(const std::string& src);
        static std::string decode(const std::string& src);

    private:
    };

    std::string coder::encode(const std::string& source)
    {
        std::stringstream out;
        std::for_each(source.rbegin(), source.rend(), [&](auto ch)
        {
            std::stringstream ss;
            ss << (int)ch;
            std::string str(ss.str());
            std::reverse(str.begin(), str.end());
            out << str;
        });
        return out.str();
    }

    std::string coder::decode(const std::string& source)
    {
        std::stringstream out;
        std::string tmp;

        for (auto it = source.begin(); it != source.end();)
        {
            tmp.assign(it, it + 2);
            it += 2;
            std::reverse(tmp.begin(), tmp.end());
            std::stringstream in(tmp);
            tmp.clear();
            int ch;
            in >> ch;

            if (ch < 32)
            {
                ++it;
                ch += 100;
            }

            out << char(ch);
        }

        tmp.assign(out.str());
        std::reverse(tmp.begin(), tmp.end());
        return tmp;
    }

}  // namespace

void U444::operator()() const
{
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (!line.empty())
        {
            std::cout << (std::isdigit(line[0]) ? coder::decode(line) : coder::encode(line));
        }

        std::cout << std::endl;
    }
}
