#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u416.h"

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
    U416 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : leds_(), match_() {}

        operator bool()
        {
            return not leds_.empty();
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        std::vector<std::string> leds_;
        bool match_;

        const size_t n_leds_ = 7;
    };

    solution_t& solution_t::operator()()
    {
        static std::vector<std::string> good( {
            "YYYYYYN",//0
            "NYYNNNN",//1
            "YYNYYNY",//2
            "YYYYNNY",//3
            "NYYNNYY",//4
            "YNYYNYY",//5
            "YNYYYYY",//6
            "YYYNNNN",//7
            "YYYYYYY",//8
            "YYYYNYY",//9
        });
        match_ = false;

        for (size_t i = 9; i >= leds_.size() - 1; --i) {
            std::vector<bool> bad(n_leds_);
            size_t j(0);

            for (; j < leds_.size(); ++j) {
                size_t k(0);

                for (; k < n_leds_; ++k) {
                    if (bad[k] && leds_[j][k] == 'Y') {
                        break;
                    }

                    if (leds_[j][k] == 'N' && good[i - j][k] == 'Y') {
                        bad[k] = true;
                    } else if (leds_[j][k] == 'Y' && good[i - j][k] == 'N') {
                        break;
                    }
                }

                if (k != n_leds_) { // loop not finished
                    break;
                }
            }

            if (j == leds_.size()) { // loop finished
                match_ = true;
                break;
            }
        }

        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.leds_.clear();
        size_t n(0);

        if (in >> n) {
            sol.leds_.reserve(n);
            std::istream_iterator<std::string> sin(in);
            std::copy_n(sin, n, std::back_inserter(sol.leds_));
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << (sol.match_ ? "" : "MIS") << "MATCH";
        return out;
    }

}

void U416::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
