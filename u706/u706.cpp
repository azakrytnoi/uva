#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u706.h"

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
    U706 instance;
    instance();
}

namespace {

class solution {
public:
    solution();

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const;

private:
    size_t size_;
    std::string number_;
    std::vector<std::vector<std::string>> digits_;
};

solution::solution() :
    size_(0), number_(), digits_(
{
    { " - ", "| |", "   ", "| |", " - " },
    { "   ", "  |", "   ", "  |", "   " },
    { " - ", "  |", " - ", "|  ", " - " },
    { " - ", "  |", " - ", "  |", " - " },
    { "   ", "| |", " - ", "  |", "   " },
    { " - ", "|  ", " - ", "  |", " - " },
    { " - ", "|  ", " - ", "| |", " - " },
    { " - ", "  |", "   ", "  |", "   " },
    { " - ", "| |", " - ", "| |", " - " },
    { " - ", "| |", " - ", "  |", " - " }
})
{}

solution::operator bool() const
{
    return size_ != 0;
}

std::istream& operator >>(std::istream& in, solution& sol)
{
    in >> sol.size_ >> sol.number_;
    return in;
}

std::ostream& operator <<(std::ostream& out, const solution& sol)
{
    size_t row(0);
    size_t col(0);
    auto printLine = [&](auto digit) {
        if ((col++) != 0) {
            out << ' ';
        }

        size_t idx = digit - '0';
        out << sol.digits_[idx][row][0];

        for (size_t i = 0; i < sol.size_; i++) {
            out << sol.digits_[idx][row][1];
        }

        out << sol.digits_[idx][row][2];
    };
    auto printScaledLine = [&]() {
        // cppcheck-suppress unreadVariable
        col = 0;
        std::for_each(sol.number_.begin(), sol.number_.end(), printLine);
        out << std::endl;
    };
    printScaledLine();
    row++;

    for (size_t ri = 0; ri < sol.size_; ri++) {
        printScaledLine();
    }

    row++;
    printScaledLine();
    row++;

    for (size_t ri = 0; ri < sol.size_; ri++) {
        printScaledLine();
    }

    row++;
    printScaledLine();
    return out;
}

}  // namespace

void U706::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol << std::endl;
    }
}
