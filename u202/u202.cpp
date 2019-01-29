#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u202.h"

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
    U202 instance;
    instance();
}

namespace {

    class solution {
        int32_t numerator_, denominator_;
        size_t repeat_term_index_;
        size_t parenthesis_start_;
        std::string before_decimal_;
        std::string after_decimal_;
    public:
        solution() : numerator_(0), denominator_(0), repeat_term_index_(), parenthesis_start_(), before_decimal_(), after_decimal_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.numerator_ >> sol.denominator_;
        sol.before_decimal_.clear();
        sol.after_decimal_.clear();
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        int32_t repeatedCycle = sol.repeat_term_index_ - sol.parenthesis_start_;
        out << sol.numerator_ << "/" << sol.denominator_ << " = ";
        out << sol.before_decimal_;

        if ( sol.after_decimal_.size () == sol.parenthesis_start_ ) {
            for ( size_t i = 0; i < sol.after_decimal_.size () - 1; i++ ) {
                out << sol.after_decimal_.at (i);
            }

            out << "(" << sol.after_decimal_.at (sol.after_decimal_.size () - 1) << ")" << std::endl;
        } else if ( sol.repeat_term_index_ > 51 ) {
            for ( size_t i = 0; i < 50; i++ ) {
                if ( i + 1 == sol.parenthesis_start_ ) {
                    out << "(";
                }

                out << sol.after_decimal_.at (i);
            }

            out << "...)" << std::endl;
        } else {
            for ( size_t i = 0; i < sol.after_decimal_.size (); i++ ) {
                if ( i + 1 == sol.parenthesis_start_ ) {
                    out << "(";
                }

                out << sol.after_decimal_.at (i);
            }

            out << ")" << std::endl;
        }

        out << "   " << repeatedCycle << " = number of digits in repeating cycle" << std::endl;
        return out;
    }

    solution& solution::operator()()
    {
        std::map<int32_t, int32_t> freq_matrix;
        int32_t quotient = numerator_ / denominator_;
        int32_t mod = (numerator_ % denominator_) * 10;
        std::stringstream num;
        num << quotient;
        before_decimal_ += num.str();
        before_decimal_ += '.';
        repeat_term_index_ = 1;

        while ( freq_matrix [mod] == 0 ) {
            freq_matrix [mod] = repeat_term_index_++;
            quotient = mod / denominator_;
            mod %= denominator_;
            after_decimal_ += (quotient + '0');
            mod *= 10;
        }

        parenthesis_start_ = freq_matrix [mod];
        return *this;
    }

}

void U202::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
