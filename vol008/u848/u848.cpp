#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u848.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U848 instance;
    instance();
}

namespace {

    class solution {
        std::vector<std::string> buffer_;

    public:
        solution() : buffer_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);

        operator bool() const
        {
            return !buffer_.empty();
        }

        std::string operator()();

    private:
        static void trim(std::string& str)
        {
//        if (str.front() == ' ') {
//            str.erase(0, str.find_first_not_of(' '));
//        }
            if (!str.empty() && str.back() == ' ')
            {
                str.erase(str.find_last_not_of(' '), std::string::npos);
            }
        }
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::string line;
        std::stringstream instream;

        while (std::getline(in, line))
        {
            solution::trim(line);

            if (line.empty())
            {
                sol.buffer_.push_back(instream.str());
                instream.str(line);
            }
            else
            {
                line += " ";
                instream << line;
            }
        }

        sol.buffer_.push_back(instream.str());
        return in;
    }

    std::string solution::operator ()()
    {
        std::stringstream outstream;
        std::for_each(buffer_.begin(), buffer_.end(), [&](auto str)
        {
            std::string line;

            if (str.length() <= 72)
            {
                line.assign(str);
            }
            else
            {
                for (auto it = str.begin(); it != str.end() && *it == ' '; ++it)
                {
                    line += ' ';
                }

                std::stringstream paragraph(str);
                std::string word;

                while (paragraph >> word)
                {
                    if ((line.length() + word.length()) < 71)
                    {
                        line += (line.empty() ? "" : " ") + word;
                    }
                    else
                    {
                        outstream << line << std::endl;
                        line = word;
                    }
                }
            }

            outstream << line << std::endl << std::endl;
        });
        return outstream.str();
    }

}

void U848::operator()() const
{
    solution sol;
    std::cin >> sol;

    if (sol)
    {
        std::cout << sol() << std::endl;
    }
}
