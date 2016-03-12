#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11541.h"

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
void __cdecl invoke() {
    U11541 instance;
    instance();
}

namespace {

    class decoder {
    public:
        decoder() :
                buffer_() {
        }

        decoder& operator <<(const std::string& source);

        friend std::ostream& operator <<(std::ostream& out, const decoder& engine) {
            out << engine.buffer_;
            return out;
        }

    private:
        std::string buffer_;
    };

    decoder& decoder::operator <<(const std::string& source) {
        std::stringstream ss;
        char ch;
        int count(0);
        for (auto it = source.begin(); it != source.end(); ++it) {
            if (std::isalpha(*it)) {
                if (count != 0) {
                    ss << std::string(count, ch);
                    count = 0;
                }
                ch = *it;
            } else if (std::isdigit (*it)) {
                count *= 10;
                count += (*it) - '0';
            }
        }
        ss << std::string(count, ch);
        buffer_.assign(ss.str());
        return *this;
    }

}  // namespace

void U11541::operator()() {
    int N;
    std::cin >> N;
    decoder engine;
    std::string line;
    std::getline(std::cin, line);
    int case_no(0);
    while (N--) {
        std::getline(std::cin, line);
        std::cout << "Case " << (++case_no) << ": " << (engine << line) << std::endl;
    }
}
