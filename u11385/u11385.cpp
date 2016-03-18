#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11385.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cctype>

#include "fibonacci.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11385 instance;
    instance();
}

namespace {

class solution {
public:
    solution() :
        fb_(), cipher_(), cipher_text_(), text_len_(0)
    {
        cipher_text_.reserve(100);
    }

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

private:
    math::fibonacci<102> fb_;
    std::vector<uint64_t> cipher_;
    std::string cipher_text_;
    size_t text_len_;
};

std::istream& operator >>(std::istream& in, solution& sol)
{
    int n;
    in >> n;
    sol.cipher_.clear();
    sol.cipher_.reserve(n);
    sol.text_len_ = 0;
    std::generate_n(std::back_inserter(sol.cipher_), n, [&]() {
        uint64_t tmp;
        in >> tmp;
        sol.text_len_ = std::max(sol.text_len_, sol.fb_(tmp));
        return tmp;
    });
    std::getline(in, sol.cipher_text_);
    std::getline(in, sol.cipher_text_);
    std::copy_if(sol.cipher_text_.begin(), sol.cipher_text_.end(), sol.cipher_text_.begin(), [](auto ch) {
        return std::isalpha(ch) && std::isupper(ch);
    });
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    std::string text(sol.text_len_, ' ');
    for (size_t i = 0; i < sol.cipher_.size(); i++) {
        text[sol.fb_(sol.cipher_[i]) - 1] = sol.cipher_text_[i];
    }
    out << text;
    return out;
}

}  // namespace

void U11385::operator()()
{
    solution sol;
    int N;
    std::cin >> N;
    while (N--) {
        std::cin >> sol;
        std::cout << sol << std::endl;
    }
}
