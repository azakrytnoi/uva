#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u227.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U227 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), change_({{'A', {-1, 0}}, {'B', {1, 0}}, {'R', {0, 1}}, {'L', {0, -1}}}), grid_(), moves_(), space_(), valid_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not moves_.empty();
        }
        solution_t& operator()();

    private:
        size_t case_no_;
        typedef std::pair<int8_t, int8_t> location_t;
        std::map<char, location_t> change_;
        std::vector<std::string> grid_;
        std::string moves_;
        location_t space_;
        bool valid_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.grid_.clear();
        sol.moves_.clear();
        sol.space_ = {-1, -1};
        sol.grid_ = std::vector<std::string>(5, std::string(5, ' '));

        for (int8_t row = 0; row < 5; ++row) {
            std::string line;
            std::getline(in, line);

            if (line == "Z") {
                return in;
            }

            std::copy(line.begin(), line.end(), sol.grid_[row].begin());

            if (sol.grid_[row].find(' ') != std::string::npos) sol.space_ = {row, sol.grid_[row].find(' ')};
        }

        char ch;

        while (in >> ch && ch != '0') {
            sol.moves_.push_back(ch);
        }

        in.ignore();

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Puzzle #" << sol.case_no_ << ':' << std::endl;

        if (sol.valid_) {
            std::for_each(sol.grid_.begin(), sol.grid_.end(), [&](const std::string & row) {
                std::string filler;
                std::for_each(row.begin(), row.end(), [&](const char ch) {
                    out << filler << ch;
                    filler = " ";
                });
                out << std::endl;
            });
        } else {
            out << "This puzzle has no final configuration." << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        valid_ = true;
        ++case_no_;

        for (auto move : moves_) {
            auto change = change_.find(move);
            valid_ = change != change_.end();

            if (valid_) {
                location_t next ({space_.first + change->second.first, space_.second + change->second.second});
                valid_ = not (next.first < 0 || next.first >= 5 || next.second < 0 || next.second >= 5);

                if (not valid_) {
                    break;
                }

                std::swap(grid_[next.first][next.second], grid_[space_.first][space_.second]);
                space_ = next;
            };
        }

        return *this;
    }

}

void U227::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
