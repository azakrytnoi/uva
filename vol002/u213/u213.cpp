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
#include <charconv>

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
        {}
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

            for (auto ch : keys)
            {
                key = key_advance(key_size, key_value);

                if (key.find_first_not_of('1') == std::string::npos)
                {
                    key_size++;
                    key_value = 0;
                    key = key_advance(key_size, key_value);
                }

                code_[key] = ch;
            };

            return *this;
        }

        friend std::istream& operator >>(std::istream& in, coder& coder)
        {
            auto readchar = [&]()
            {
                char ch;
                in >> ch;
                return ch;
            };
            std::string message;
            std::string block;
            std::string chunk;

            do
            {
                block.clear();
                std::generate_n(std::back_inserter(block), 3, readchar);
                size_t block_len;
                std::from_chars(block.data(), block.data() + 3, block_len, 2);

                if (block_len > 0)
                {
                    while (true)
                    {
                        chunk.clear();
                        std::generate_n(std::back_inserter(chunk), block_len, readchar);

                        if (chunk.find('0') == std::string::npos)
                        {
                            break;
                        }

                        message += coder.code_[chunk];
                    }
                }
            }
            while (block != "000");

            coder.message_ = message;
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
            std::string result (key_size, '0');

            if (uint16_t val (key_value++); val != 0)
            {
                char buff[std::numeric_limits<uint16_t>::digits + 1];
                auto cnv = std::to_chars(buff, buff + sizeof(buff), val, 2);
                auto len = cnv.ptr - buff;
                std::copy_n(buff, len, result.end() - len);
            }

            return result;
        }
    };

}  // namespace

void U213::operator()() const
{
    coder coder;
    std::string head;

    while (std::cin && std::getline(std::cin, head))
    {
        coder = head;
        std::cin >> coder;
        std::cout << coder << std::endl;
        std::getline(std::cin, head);
    }
}
