#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u740.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U740 instance;
    instance();
}

namespace
{
class baudot
{
    enum class shift
    {   UP, DOWN };
    shift shift_;
    std::map<shift, std::string> code_;

public:
    baudot() : shift_(shift::DOWN), code_() {}

    friend std::istream& operator >> (std::istream& in, baudot& eng);

    friend std::string operator << (baudot& eng, const std::string& input);
};

std::istream& operator >> (std::istream& in, baudot& eng)
{
    eng.shift_ = baudot::shift::DOWN;
    std::getline(in, eng.code_[baudot::shift::DOWN]);
    std::getline(in, eng.code_[baudot::shift::UP]);
    return in;
}

std::string operator << (baudot& eng, const std::string& input)
{
    eng.shift_ = baudot::shift::DOWN;
    std::string result;
    for (auto it = input.begin(); it != input.end(); ) {
        uint16_t code(0);
        for (size_t power = 0x10; power > 0; power >>= 1, ++it) {
            if (*it == '1') {
                code += uint16_t(power);
            }
        }
        switch (code) {
        case 0b11011:
            eng.shift_ = baudot::shift::DOWN;
            break;
        case 0b11111:
            eng.shift_ = baudot::shift::UP;
            break;
        default:
            result += eng.code_[eng.shift_][code];
            break;
        }
    }
    return result;
}
} // namespace

void U740::operator()() const
{
    baudot eng;
    std::cin >> eng;
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::cout << (eng << line) << std::endl;
    }
}
