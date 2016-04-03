#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10921.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>
#include <cctype>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10921 instance;
    instance();
}

namespace
{

class coder
{
public:

    coder() : buffer_() {}

    coder& operator << (const std::string& input);
    coder& operator >> (std::string& output);

private:
    std::string buffer_;
};

coder& coder::operator <<(const std::string& input)
{
    std::stringstream oss;
    std::ostream_iterator<char> ois(oss, "");
    std::transform(input.begin(), input.end(), ois, [](auto ch) {
        switch (std::toupper(ch)) {
        case 'A':
        case 'B':
        case 'C':
            return '2';
        case 'D':
        case 'E':
        case 'F':
            return '3';
        case 'G':
        case 'H':
        case 'I':
            return '4';
        case 'J':
        case 'K':
        case 'L':
            return '5';
        case 'M':
        case 'N':
        case 'O':
            return '6';
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
            return '7';
        case 'T':
        case 'U':
        case 'V':
            return '8';
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            return '9';
        default:
            return ch;
        }
    });
    buffer_.assign(oss.str());
    return *this;
}

coder& coder::operator >>(std::string& output)
{
    output.assign(buffer_);
    return *this;
}

}  // namespace

void U10921::operator()()
{
    std::string line;
    coder coder;
    while (std::getline(std::cin, line) && !line.empty()) {
        (coder << line) >> line;
        std::cout << line << std::endl;
    }
}
