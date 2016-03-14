#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u619.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cctype>
#include <sstream>

#include "bigint.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke() {
    U619 instance;
    instance();
}

namespace {
    class checker {
    public:
        static std::string translate(const std::string& source);

    private:
        static std::string change_base(const std::string& source, int from, int to, int base);
        static void process_carry(std::vector<int>& data, int to);

        template<typename T>
        static void print(std::ostream& out, T& data);
    };

    template<typename T>
    void checker::print(std::ostream& out, T& data) {
        size_t pos;
        bool printed = false;
        for (pos = 0; (data.size() - pos) % 3 != 0; pos++) {
            out << data[pos];
            printed = true;
        }
        for (; pos < data.size(); pos += 3) {
            if (printed) {
                out << ',';
            }
            out << data[pos] << data[pos + 1] << data[pos + 2];
            printed = true;
        }
    }

    std::string checker::translate(const std::string & source) {
        if (std::isdigit(source[0])) {
            return change_base(source, 10, 26, '0');
        }
        return change_base(source, 26, 10, 'a' - 1);
    }

    std::string checker::change_base(const std::string& source, int from, int to, int base) {
        std::vector<int> internal;
        internal.push_back(0);
        std::for_each(source.begin(), source.end(), [&](auto ch) {
            std::transform(internal.begin(), internal.end(), internal.begin(), [&] (auto digit) {return digit * from;});
            int last = internal.back() + ch - base;
            internal[internal.size() - 1] = last;
            process_carry(internal, to);
        });
        std::stringstream out;
        if (from == 10) {
            std::string translated;
            translated.resize(internal.size());
            std::transform(internal.begin(), internal.end(), translated.begin(), [](auto ch) {return ch + 'a' - 1;});
            out.setf(std::ios::left);
            out << std::setw(22) << translated;
            print(out, source);
        } else {
            out.setf(std::ios::left);
            out << std::setw(22) << source;
            print(out, internal);
        }
        return out.str();
    }

    void checker::process_carry(std::vector<int>& data, int base_to) {
        for (auto it = data.rbegin(); it != data.rend() - 1; ++it) {
            *(it + 1) += *it / base_to;
            *it %= base_to;
        }
        int tmp = data[0];
        while (tmp >= base_to) {
            data.insert(data.begin(), tmp / base_to);
            data[1] = tmp % base_to;
            tmp /= base_to;
        }
    }

}

void U619::operator()() {
    std::string line;
    while (std::getline(std::cin, line) && line != "*") {
        std::cout << checker::translate(line) << std::endl;
    }
}
