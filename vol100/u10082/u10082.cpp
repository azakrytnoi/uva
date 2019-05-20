#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10082.h"

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
    U10082 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : digits_("`1234567890-="), letters_("QWERTYUIOP[]\\ASDFGHJKL;'ZXCVBNM,./"), message_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        const std::string digits_;
        const std::string letters_;
        std::string message_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        std::getline(in, sol.message_);
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.message_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::transform(message_.begin(), message_.end(), message_.begin(), [&](auto ch) -> char
        {
            auto pos = std::find(digits_.begin(), digits_.end(), ch);

            if (pos != digits_.end())
            {
                return *(--pos);
            }

            pos = std::find(letters_.begin(), letters_.end(), ch);

            if (pos != letters_.end())
            {
                return *(--pos);
            }

            return ch;
        });
        return *this;
    }

}

void U10082::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
