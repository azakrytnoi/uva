#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u739.h"

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
    U739 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : name_(), code_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return not name_.empty();
        }
        solution& operator()();

    private:
        std::string name_;
        std::string code_;

        uint16_t encode (char ch);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.code_.clear();
        sol.name_.clear();
        in >> sol.name_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << std::setw(9) << ' ' << std::left << std::setw(25) << sol.name_ << std::setw(1) << sol.code_;
        return out;
    }

    solution& solution::operator()()
    {
        auto current = name_.begin();
        std::stringstream code;
        code << *(current);
        size_t code_len(0);
        auto last = encode(*(current++));

        for (; current != name_.end() && code_len < 3; ++current) {
            auto curr = encode(*(current));

            if (curr != last && curr != 0) {
                code << curr;
                code_len++;
            }

            last = curr;
        }

        while ((code_len++) < 3) {
            code << '0';
        }

        code_ = code.str();
        return *this;
    }

    uint16_t solution::encode(char ch)
    {
        static std::string soundex[] = {
            "AEIOUYWH",
            "BPFV",
            "CSKGJQXZ",
            "DT",
            "L",
            "MN",
            "R"
        };

        for (size_t idx = 0; idx < sizeof soundex; idx++) {
            if (soundex[idx].find(ch) != std::string::npos) {
                return idx;
            }
        }

        return -1;
    }

}

void U739::operator()() const
{
    solution sol;
    std::cout << std::setw(9) << ' ' << std::left << std::setw(25) << "NAME" << std::setw(1) << "SOUNDEX CODE" << std::endl;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }

    std::cout << std::setw(19) << ' ' << std::setw(1) << "END OF OUTPUT" << std::endl;
}
