#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u306.h"

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
void __cdecl invoke() {
    U306 instance;
    instance();
}

namespace {

    class cipher {
        std::vector<int> cipher_;
        std::string buffer_;
    public:
        cipher() :
                cipher_(), buffer_() {
        }

        friend std::istream& operator >>(std::istream& in, cipher& cipher) {
            cipher.cipher_.clear();
            int n;
            in >> n;
            cipher.cipher_.reserve(n);
            std::generate_n(std::back_inserter(cipher.cipher_), n, [&]() {int tmp; in >> tmp; return tmp;});
            cipher.buffer_.reserve(n);
            return in;
        }

        operator bool() const {
            return !cipher_.empty();
        }

        cipher& operator <<(const std::string& src_message);
        cipher& operator >>(std::string& dst_message);

    private:
    };

    cipher& cipher::operator <<(const std::string& src) {
        buffer_.clear();
        buffer_.resize(cipher_.size());
        auto it = src.begin();
        std::for_each(cipher_.begin(), cipher_.end(), [&](auto idx) {buffer_[idx - 1] = (it == src.end() ? ' ' : *it++); });
        return *this;
    }

    cipher& cipher::operator >>(std::string& dst) {
        dst.assign(buffer_);
        buffer_.clear();
        return *this;
    }

}  // namespace

void U306::operator()() {
    cipher cipher;
    while ((std::cin >> cipher) && cipher) {
        int k;
        while ((std::cin >> k) && k > 0) {
            std::string line;
            std::getline(std::cin, line);
            line.erase(0, 1);
            while (k--) {
                (cipher << line) >> line;
            }
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }
}
