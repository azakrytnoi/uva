#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u445.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U445 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : source_(), maze_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not source_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<std::string> source_;
        std::vector<std::string> maze_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.source_.clear();
        std::string line;

        while (std::getline(in, line) && not line.empty())
        {
            sol.source_.push_back(line);
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<std::string> sout(out, "\n");
        std::copy(sol.maze_.begin(), sol.maze_.end(), sout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        maze_.clear();

        for (auto& src : source_)
        {
            size_t n(0);
            std::string line;

            for (auto ch : src)
            {
                switch (ch)
                {
                case '!':
                    maze_.push_back(line);
                    line.clear();
                    n = 0;
                    break;

                default:
                    if (std::isdigit(ch))
                    {
                        n += ch - '0';
                    }
                    else
                    {
                        line += std::string(n, ch == 'b' ? ' ' : ch);
                        n = 0;
                    }

                    break;
                }
            }

            maze_.push_back(line);
        }

        return *this;
    }

}

void U445::operator()() const
{
    solution_t sol;

    do
    {
        std::cin >> sol;

        if (sol)
        {
            std::cout << sol() << std::endl;
        }
    }
    while (sol);
}
