#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10878.h"

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
    U10878 instance;
    instance();
}

namespace
{

class coder
{
public:
    static char decode(const std::string& line);

private:
};

char coder::decode(const std::string& line)
{
    std::string temp(line.substr(1, 9));
    char code = std::accumulate(temp.begin(), temp.end(), 0, [](auto ch, auto current) {
        if (current != '.') {
            ch <<= 1;
            ch |= current == 'o' ? 1 : 0;
        }
        return ch;
    });
    return code;
}

}  // namespace

void U10878::operator()()
{
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty() && line != "___________") {
            std::cout << coder::decode(line);
        }
    }
}
