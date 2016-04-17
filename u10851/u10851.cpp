#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10851.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <array>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10851 instance;
    instance();
}

namespace
{
class decoder
{
public:
    decoder() :
        message_()
    {
    }

    friend std::istream& operator >>(std::istream& in, decoder& dec);
    friend std::ostream& operator <<(std::ostream& out, const decoder& dec);

    decoder& operator <<(const std::vector<std::string>& input);

private:
    std::string message_;
};

std::istream& operator >>(std::istream& in, decoder& dec)
{
    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> input;
    input.reserve(10);
    std::generate_n(std::back_inserter(input), 10, [&]() -> std::string {
        std::getline(std::cin, line);
        // cppcheck-suppress returnReference
        return line;
    });
    dec << input;
    return in;
}

std::ostream& operator <<(std::ostream& out, const decoder& dec)
{
    out << dec.message_;
    return out;
}

decoder& decoder::operator <<(const std::vector<std::string>& input)
{
    message_.clear();
    message_.reserve(input[0].length() - 1);
    for (size_t i = 1; i < input[0].length(); i++) {
        char ch(0);
        for (size_t j = 0, power = 1; j < 9; j++, power <<= 1) {
            if (input[j][i] == '\\') {
                ch += char(power >> 1);
            }
        }
        message_ += ch;
    }

    return *this;
}
}  // namespace

void U10851::operator()() const
{
    int N;
    std::cin >> N;
    decoder dec;
    while (N--) {
        std::cin >> dec;
        std::cout << dec << std::endl;
    }
}
