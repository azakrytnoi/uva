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
            auto led = leds_.begin();

            for (; led != leds_.end(); ++led) {
                auto possible = good[i - std::distance(leds_.begin(), led)].begin();
                auto bad_led = bad.begin();
                auto segment = led->begin();

                for (; segment != led->end(); ++segment, ++bad_led, ++possible) {
                    if (*bad_led && *segment == 'Y') {
                        break;
                    }

                    if (*segment != *possible) {
                        if (*segment == 'N') {
                            *bad_led = true;
                        } else if (*segment == 'Y') {
                            break;
                        }
                    }
                }

                if (segment != led->end()) { // loop not finished
                    break;
                }
            }

            if (led == leds_.end()) { // loop finished
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
