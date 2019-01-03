#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u632.h"

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
    U632 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : original_(), result_(), s1_pos_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        std::string original_;
        std::string result_;
        size_t s1_pos_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        size_t n;
        in >> n;
        sol.original_.reserve(n);
        std::generate_n(std::back_inserter(sol.original_), n, [&]() {
            char ch;

            while (in.get(ch) && ch == '\n');

            return ch;
        });
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.s1_pos_ << std::endl;
        std::ostream_iterator<char> oit (out, "");
        size_t line_length(0);
        std::transform(sol.result_.begin(), sol.result_.end(), oit, [&](char ch) {

            if (line_length >= 50) {
                out << std::endl;
                line_length = 0;
            }

            line_length++;
            return ch;
        });
        out << std::endl;
        return out;
    }

    solution& solution::operator()()
    {
        std::vector<std::pair<size_t, std::string>> sn;
        sn.reserve(original_.length());
        size_t seq_no(0);
        std::generate_n(std::back_inserter(sn), original_.length(), [&]() {
            auto pair = std::make_pair(seq_no++, original_);
            auto ch = original_[0];
            original_ = original_.erase(0, 1) + ch;
            return pair;
        });
        std::sort(sn.begin(), sn.end(), [](const std::pair<size_t, std::string>& lhs, const std::pair<size_t, std::string>& rhs) {
            return lhs.second < rhs.second;
        });
        result_.reserve(original_.length());
        size_t row_no(0);

        for (auto& pair : sn) {
            if (pair.first == 1) {
                s1_pos_ = row_no;
            }

            result_ += *pair.second.rbegin();
            row_no++;

        }

        return *this;
    }

}

void U632::operator()() const
{
    size_t N;
    std::cin >> N;

    while (N--) {
        solution sol;
        std::cin >> sol;
        std::cout << sol();

        if (N != 0) {
            std::cout << std::endl;
        }
    }
}
