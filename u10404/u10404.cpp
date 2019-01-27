#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10404.h"

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
    U10404 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : total_(), stones_(), winner_() {}

        operator bool()
        {
            return total_ != 0;;
        }

        solution_t& operator()()
        {
            std::vector<uint32_t> dp(total_ + 1);
            std::sort(stones_.begin(), stones_.end());

            for (auto dpi = dp.begin(); dpi != dp.end(); ++dpi) {
                for (auto sti = stones_.begin(); sti != stones_.end(); ++sti) {
                    if (std::distance(dp.begin(), dpi) >= *sti) {
                        if (*(dpi - *sti) == 0) {
                            *dpi = 1;
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }

            winner_ = *(dp.end() - 1) == 1;
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            size_t m;
            sol.total_ = 0;
            sol.winner_ = false;
            sol.stones_.clear();

            if (in >> sol.total_ >> m) {
                sol.stones_.reserve(m);
                std::generate_n(std::back_inserter(sol.stones_), m, [&]() {
                    int32_t tmp;
                    in >> tmp;
                    return tmp;
                });
            }

            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << (sol.winner_ ? "Stan" : "Ollie") << " wins";
            return out;
        }

    private:
        uint32_t total_;
        std::vector<int32_t> stones_;
        bool winner_;
    };

}

void U10404::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
