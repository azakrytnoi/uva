#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10132.h"

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
    U10132 instance;
    instance();
}

namespace {
class solution {
    std::vector<std::string> fragments_;
    std::string result_;
    size_t bits_;
public:
    solution() : fragments_(), result_(), bits_(0) {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return !fragments_.empty();
    }
    solution& operator()();

private:
    static bool matchAll(const std::string& result, std::vector<std::string> fragments)
    {
        size_t nMatched = 0;

        for (size_t i = 0; i < fragments.size(); i++) {
            for (size_t j = 0; j < fragments.size(); j++) {
                if (i != j && (fragments[i] + fragments[j]) == result) {
                    nMatched++, fragments[i] = "*", fragments[j] = "*";
                }
            }
        }

        if (nMatched == fragments.size() / 2) {
            return true;
        }

        return false;
    }
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.result_.clear();
    sol.fragments_.clear();
    sol.bits_ = 0;
    std::string line;

    while (in >> line && !line.empty()) {
        sol.fragments_.push_back(line);
        sol.bits_ += line.size();
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << sol.result_;
    return out;
}

solution& solution::operator ()()
{
    size_t codeSize = (bits_ * 2) / fragments_.size();
    std::string candidate;

    for (size_t i = 0; i < fragments_.size(); i++) {
        for (size_t j = 0; j < fragments_.size(); j++) {
            if (i != j &&  fragments_[i].length() + fragments_[j].length() == codeSize) {
                candidate.assign(fragments_[i]).append(fragments_[j]);

                if (matchAll(candidate, fragments_)) {
                    result_ = candidate;
                    return *this;
                }
            }
        }
    }

    return *this;
}
}

void U10132::operator()() const
{
    solution sol;
    int ncases;
    std::cin >> ncases;

    while (ncases--) {
        std::cin >> sol;

        if (sol) {
            std::cout << sol() << std::endl;
        }
    }
}
