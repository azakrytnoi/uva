#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11697.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <array>
#include <iterator>
#include <numeric>
#include <limits>
#include <cctype>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U11697 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : key_(), plain_(), encoded_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        std::string key_;
        std::string plain_;
        std::string encoded_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::getline(in, sol.key_);
        std::getline(in, sol.plain_);
        sol.encoded_.clear();
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.encoded_;
        return out;
    }

    solution& solution::operator()()
    {
        std::array<std::array<char, 5>, 5> matrix;
        std::set<char> processed;
        size_t i(0), j(0);
        auto fill_matrix = [&](char ch)
        {
            if (std::isalpha(ch) && processed.find(ch) == processed.end())
            {
                matrix[i][j] = std::toupper(ch);
                processed.insert(ch);
                j++;
                j %= 5;

                if (j == 0)
                {
                    i++;
                    i %= 5;
                }
            }
        };

        for (char ch : key_)
        {
            fill_matrix(ch);
        }

        for (char ch = 'a'; ch <= 'z'; ch++)
        {
            if (ch == 'q')
            {
                continue;
            }

            fill_matrix(ch);
        }

        auto locate = [&](char ch) -> std::pair<size_t, size_t>
        {
            for (size_t row = 0; row < 5; row++)
            {
                for (size_t col = 0; col < 5; col++)
                {
                    if (matrix[row][col] == ch)
                    {
                        return std::make_pair(row, col);
                    }
                }
            }

            return std::make_pair(0, 0);
        };

        std::string work;
        work.reserve(plain_.length());
        std::copy_if(plain_.begin(), plain_.end(), std::back_inserter(work), [](const char ch)
        {
            return std::isalpha(ch);
        });
        auto ptr = work.begin();
        auto make_digraph = [&]() -> std::pair<char, char>
        {
            char ch1 = std::toupper(*(ptr++));
            char ch2 = ptr == work.end() ? ch1 : std::toupper(*ptr);

            if (ch1 == ch2)
            {
                ch2 = 'X';
            }
            else
            {
                ++ptr;
            }

            return std::make_pair(ch1, ch2);
        };

        while (ptr != work.end())
        {
            auto digraph = make_digraph();
            auto loc1 = locate(digraph.first);
            auto loc2 = locate(digraph.second);

            if (loc1.first == loc2.first)
            {
                digraph.first = matrix[loc1.first][(loc1.second + 1) % 5];
                digraph.second = matrix[loc2.first][(loc2.second + 1) % 5];
            }
            else if (loc1.second == loc2.second)
            {
                digraph.first = matrix[(loc1.first + 1) % 5][loc1.second];
                digraph.second = matrix[(loc2.first + 1) % 5][loc2.second];
            }
            else
            {
                digraph.first = matrix[loc1.first][loc2.second];
                digraph.second = matrix[loc2.first][loc1.second];
            }

            encoded_ += digraph.first;
            encoded_ += digraph.second;
        }

        return *this;
    }

}

void U11697::operator()() const
{
    size_t N;
    solution sol;
    std::cin >> N;
    std::string dummy;
    std::getline(std::cin, dummy);

    while (N--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
