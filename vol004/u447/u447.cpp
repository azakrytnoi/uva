#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u447.h"

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
    U447 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : years_(), //
            // deepcode ignore : Unclear what the issue is
            map_(std::vector<std::vector<std::string>>(2, std::vector<std::string>(MAX_, std::string(MAX_, ' ')))), //
            current_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return years_-- > 1;
        }
        solution_t& operator()();

        const static int64_t MAX_ = 20;
    private:
        size_t years_;
        std::vector<std::vector<std::string>> map_;
        size_t current_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.years_;
        in.ignore();
        std::string line;

        while (std::getline(in, line) && not line.empty())
        {
            std::stringstream temp(line);
            size_t x(0), y(0);
            temp >> x >> y;
            sol.map_[sol.current_][x - 1][y - 1] = 'O';
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << std::string(sol.MAX_, '*') << std::endl;
        std::ostream_iterator<std::string> sout(out, "\n");
        std::copy(sol.map_[sol.current_].begin(), sol.map_[sol.current_].end(), sout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto& current(map_[current_]);
        auto& next(map_[1 - current_]);
        auto neighours = [&](const int64_t x0, const int64_t y0)
        {
            size_t ncnt(0);

            for (int64_t dx = -1; dx <= 1; ++dx)
            {
                for (int64_t dy = -1; dy <= 1; ++dy)
                {
                    int64_t x(x0 + dx), y(y0 + dy);

                    if (x >= 0 && x < MAX_ && y >= 0 && y < MAX_)
                    {
                        ncnt += current[x][y] == 'O' ? 1 : 0;
                    }
                }
            }

            return current[x0][y0] == 'O' ? ncnt - 1 : ncnt;
        };

        for (size_t x = 0; x < MAX_; ++x)
        {
            for (size_t y = 0; y < MAX_; ++y)
            {
                switch (neighours(x, y))
                {
                case 2:
                    next[x][y] = current[x][y];
                    break;

                case 3:
                    next[x][y] = 'O';
                    break;

                default:
                    next[x][y] = ' ';
                    break;
                }
            }
        }

        current_ = 1 - current_;
        return *this;
    }

}

void U447::operator()() const
{
    size_t N(0);
    std::cin >> N;

    while (N--)
    {
        solution_t sol;
        std::cin >> sol;
        std::cout << sol;

        while (sol)
        {
            std::cout << sol();
        }

        std::cout << std::string(solution_t::MAX_, '*') << std::endl << std::endl;
    }
}
