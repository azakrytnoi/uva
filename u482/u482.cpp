#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u482.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

U482::U482()
{
}

U482::~U482()
{
}

namespace
{
struct item {
    uint32_t idx;
    std::string value;

    item() : idx(0), value() {}

    bool operator < (const item& other) const
    {
        return idx < other.idx;
    }
};
}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U482 instance;
    instance();
}
void U482::operator()()
{
    uint32_t n;
    std::cin >> n;
    std::string line;
    std::getline(std::cin, line);
    std::vector<item> src;
    while (n--) {
        src.clear();
        std::getline(std::cin, line);
        std::getline(std::cin, line);
        // issues with GCC 5.2
        {
            std::stringstream ssin(line);
            std::transform(std::istream_iterator<uint32_t>(ssin), std::istream_iterator<uint32_t>(), std::back_inserter(src),
            [&](auto n) {
                item i;
                i.idx = n;
                return i;
            });
        }
        std::getline(std::cin, line);
        {
            auto src_pointer = src.begin();
            std::stringstream ssin(line);
            std::for_each(std::istream_iterator<std::string>(ssin), std::istream_iterator <std::string>(),
            [&](const std::string& val) {
                (src_pointer++)->value = val;
            });
        }
        std::sort(src.begin(), src.end());
        std::for_each(src.begin(), src.end(), [&](item& itm) {
            std::cout << itm.value << std::endl;
        });
        std::cout << std::endl;
    }
}
