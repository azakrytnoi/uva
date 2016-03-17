#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u213.h"

#include <iostream>
#include <iomanip>
#include <iosfwd>
#include <vector>
#include <string>
#include <sstream>
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
    U213 instance;
    instance();
}

namespace {

class coder {
    std::map<std::string, char> code_;
    std::string message_;
public:
    coder() :
        code_(), message_()
    {
    }
    coder(coder&& rhs) :
        code_(), message_()
    {
        std::swap(code_, rhs.code_);
    }

    coder& operator =(const std::string& keys)
    {
        code_.clear();
        std::string key;
        size_t key_size = 1;
        uint16_t key_value = 0;
        std::for_each(keys.begin(), keys.end(), [&](auto ch) {
            key = key_advance(key_size, key_value);
            if (key.find_first_not_of('1') == std::string::npos) {
                key_size++;
                key_value = 0;
                key = key_advance(key_size, key_value);
            }
            code_[key] = ch;
        });
        return *this;
    }

    friend std::istream& operator >>(std::istream& in, coder& coder)
    {
        auto readchar = [&]() {
            char ch;
            in >> ch;
            return ch;
        };
        std::stringstream message;
        std::string block;
        do {
            block.clear();
            std::generate_n(std::back_inserter(block), 3, readchar);
            size_t block_len = std::accumulate(block.begin(), block.end(), 0, [](auto curr, auto ch) {
                curr <<= 1;
                return curr +  ch - '0';
            });
            if (block_len > 0) {
                std::string chunk;
                while (true) {
                    chunk.clear();
                    std::generate_n(std::back_inserter(chunk), block_len, readchar);
                    if (chunk.find('0') == std::string::npos) {
                        break;
                    }
                    message << coder.code_[chunk];
                }
            }
        } while (block != "000");
        coder.message_ = message.str();
        return in;
    }

    friend std::ostream& operator <<(std::ostream& out, const coder& coder)
    {
        out << coder.message_;
        return out;
    }

private:
    static std::string key_advance(size_t key_size, uint16_t& key_value)
    {
        std::stringstream ss;
        uint16_t val (key_value++);
        while (key_size--) {
            ss << char((val & 0x01) + '0');
            val >>= 1;
        }
        std::string result(ss.str());
        std::reverse(result.begin(), result.end());
        return result;
    }
};

}  // namespace

void U213::operator()()
{
    coder coder;
    while (std::cin) {
        std::string head;
        if (std::getline(std::cin, head)) {
            coder = head;
            std::cin >> coder;
            std::cout << coder << std::endl;
            std::getline(std::cin, head);
        }
    }
}
