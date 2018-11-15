#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10424.h"

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
    U10424 instance;
    instance();
}

namespace {

class calculator {
public:
    static int summ(const std::string& str);

private:
    static int summ(int part);
};

class solution {
    double grade_;
public:

    solution() : grade_(0) {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator << (std::ostream& out, const solution& sol);
};

int calculator::summ(const std::string& str)
{
    int part = std::accumulate(str.begin(), str.end(), 0, [](auto tot, auto ch) {
        return tot + (ch - 'a' + 1);
    });
    return part == 0 ? 1 : summ(part);
}

int calculator::summ(int part)
{
    if (part > 9) {
        int sum (0);

        while (part > 0) {
            sum += part % 10;
            part /= 10;
        }

        return summ(sum);
    }

    return part;
}

std::istream& operator >> (std::istream& in, solution& sol)
{
    auto readString = [&in]() -> std::string {
        std::string tmp;
        std::getline(in, tmp);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](auto ch)
        {
            return ::tolower(ch);
        });
        std::string res;
        std::copy_if(tmp.begin(), tmp.end(), std::back_inserter(res), [](auto ch)
        {
            return ::isalpha(ch);
        });
        // cppcheck-suppress returnReference
        return res;
    };
    std::string a (readString());
    std::string b (readString());

    if (a.empty() && b.empty()) {
        sol.grade_ = std::numeric_limits<double>::infinity();

    } else if (a.empty() || b.empty()) {
        sol.grade_ = 0.0;

    } else {
        sol.grade_ = double(calculator::summ(a)) / double(calculator::summ(b));
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    if (sol.grade_ != std::numeric_limits<double>::infinity()) {
        out << std::fixed << std::setprecision(2) << (sol.grade_ > 1.0 ? 100.0 / sol.grade_ : sol.grade_ * 100.0) << " %";
    }

    return out;
}

}  // namespace

void U10424::operator()() const
{
    solution sol;

    while (std::cin >> sol) {
        std::cout << sol << std::endl;
    }
}
