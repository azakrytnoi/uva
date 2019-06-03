#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u856.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U856 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution() : message_(), possible_(), matrix_(), results_()
        {
            std::string abc;
            std::generate_n(std::back_inserter(abc), 'Z' - 'A' + 1, []()
            {
                static char ch('A');
                return ch++;
            });
            std::generate_n(std::back_inserter(possible_), 1000, []()
            {
                static uint16_t n(999);
                static std::vector<std::string> numbers({"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"});
                uint16_t _n = n--;
                uint16_t _1 = _n % 10;
                _n /= 10;
                uint16_t _2 = _n % 10;
                _n /= 10;
                return numbers[_n] + numbers[_2] + numbers[_1];
            });
            std::generate_n(std::back_inserter(matrix_), 'Z' - 'A' + 1, [&abc]()
            {
                auto ch = abc[0];
                abc.erase(0, 1);
                abc += ch;
                return abc;
            });
        }

        operator bool ()
        {
            return not message_.empty();
        }

        solution& operator()()
        {
            for (auto& candidate : possible_)
            {
                if (candidate.length() == message_.length())
                {
                    std::string passwd(candidate.length(), ' ');

                    for (size_t idx = 0; idx < candidate.length(); idx++)
                    {
                        for (size_t pidx = 0; pidx < matrix_.size(); pidx++)
                        {
                            size_t loc = matrix_[pidx].find(message_[idx]);

                            if (static_cast<char>('a' + loc) == candidate[idx])
                            {
                                passwd[idx] = static_cast<char>('A' + pidx);
                                break;
                            }
                        }
                    }

                    results_.push_back(std::make_pair(passwd, candidate));
                }
            }

            return *this;
        }

        friend std::istream& operator >> (std::istream& in, solution& sol)
        {
            sol.message_.clear();
            sol.results_.clear();
            std::string line;

            if (std::getline(in, line) && not line.empty())
            {
                sol.message_.assign(line.begin(), line.end());
            }

            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const solution& sol)
        {
            std::ostream_iterator<std::string> iout(out, "\n");
            std::transform(sol.results_.begin(), sol.results_.end(), iout, [](const std::pair<std::string, std::string>& res)
            {
                return res.first + " -> " + res.second;
            });
            return out;
        }

    private:
        std::string message_;
        std::vector<std::string> possible_;
        std::vector<std::string> matrix_;
        std::vector<std::pair<std::string, std::string>> results_;
    };

}

void U856::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
