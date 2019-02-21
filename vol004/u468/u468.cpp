#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u468.h"

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
    U468 instance;
    instance();
}

namespace {

    class coder {
        std::vector<std::pair<char, char>> freq_;
    public:
        coder() :
            freq_()
        {}

        std::string operator()(std::string& input);
        void init(const std::string& plain);
    private:
    };

    void coder::init(const std::string& plain)
    {
        std::map<char, int> base;
        std::for_each(plain.begin(), plain.end(), [&](auto ch) {
            base[ch]++;
        });
        std::map<int, char> reverse;
        std::for_each(base.begin(), base.end(), [&](auto pair) {
            reverse[pair.second] = pair.first;
        });
        freq_.clear();
        freq_.resize(base.size());
        std::transform(reverse.rbegin(), reverse.rend(), freq_.begin(), [](auto pair) {
            return std::make_pair(pair.second, 0);
        });
    }

    std::string coder::operator ()(std::string& input)
    {
        std::map<char, int> base;
        std::for_each(input.begin(), input.end(), [&](auto ch) {
            base[ch]++;
        });
        std::map<int, char> reverse;
        std::for_each(base.begin(), base.end(), [&](auto pair) {
            reverse[pair.second] = pair.first;
        });
        auto it = freq_.begin();
        std::for_each(reverse.rbegin(), reverse.rend(), [&](auto pair) {
            (it++)->second = pair.second;
        });
        std::transform(input.begin(), input.end(), input.begin(), [&](auto ch) {
            auto iit = std::find_if(freq_.begin(), freq_.end(), [&](auto pair) {
                return pair.second == ch;
            });
            return iit->first;
        });
        return input;
    }

}  // namespace

void U468::operator()() const
{
    int N;
    std::cin >> N;
    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    coder engine;

    while (N--) {
        std::getline(std::cin, line);
        engine.init(line);
        std::getline(std::cin, line);
        std::cout << engine(line) << std::endl;
        std::getline(std::cin, line);
        std::cout << std::endl;
    }
}
