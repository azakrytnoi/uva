#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u850.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U850 instance;
    instance();
}

namespace
{
class solution
{
    char mappings_[128];
    std::vector<std::string> alphabetTokens_;
    std::vector<std::vector<std::string>> inputTokens_;
    bool possible_;
public:
    solution();

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return true;
    }
    solution& operator()();

private:

    bool mapLetters(std::vector<std::string>& encrypted)
    {
        char rmappings[128];
        std::fill_n(mappings_, 128, 0);
        std::fill_n(rmappings, 128, 0);

        for (size_t i = 0; i < alphabetTokens_.size(); i++) {
            for (size_t j = 0; j < alphabetTokens_[i].length(); j++) {
                if ((mappings_[encrypted[i][j]] == 0 && rmappings[alphabetTokens_[i][j]] == 0)
                        || (mappings_[encrypted[i][j]] == alphabetTokens_[i][j])) {
                    mappings_[encrypted[i][j]] = alphabetTokens_[i][j];
                } else {
                    return false;
                }
            }
        }

        return true;
    }
};

solution::solution() : alphabetTokens_(), inputTokens_(), possible_(false)
{
    std::stringstream ss("the quick brown fox jumps over the lazy dog");
    std::istream_iterator<std::string> iss(ss);
    std::copy(iss, std::istream_iterator<std::string>(), std::back_inserter(alphabetTokens_));
    alphabetTokens_.shrink_to_fit();
    std::fill_n(mappings_, 128, 0);
    inputTokens_.reserve(100);
}

std::istream& operator >> (std::istream& in, solution& sol)
{
    std::string line;
    sol.inputTokens_.clear();
    while (std::getline(in, line) && !line.empty()) {
        sol.inputTokens_.resize(sol.inputTokens_.size() + 1);
        std::stringstream ss(line);
        std::istream_iterator<std::string> iss(ss);
        std::copy(iss, std::istream_iterator<std::string>(), std::back_inserter(sol.inputTokens_.back()));
        sol.inputTokens_.back().shrink_to_fit();
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    if (sol.possible_) {
        for (auto iti = sol.inputTokens_.begin(); iti != sol.inputTokens_.end(); ++iti) {
            std::ostream_iterator<std::string> oit(out, " ");
            std::vector<std::string> translated;
            translated.reserve(iti->size());
            std::for_each(iti->begin(), iti->end(), [&](const std::string& element) {
                std::string result;
                std::transform(element.begin(), element.end(), std::back_inserter(result), [&](auto ch) {
                    return sol.mappings_[ch];
                });
                translated.push_back(result);
            });
            std::copy(translated.begin(), translated.end(), oit);
            out.seekp(-1, std::ios::end);
            out << std::endl;
        }
    } else {
        out << "No solution." << std::endl;
    }

    return out;
}

solution& solution::operator()()
{
    possible_ = false;
    for (auto iti = inputTokens_.begin(); !possible_ && iti != inputTokens_.end(); ++iti) {
        if (iti->size() == alphabetTokens_.size()) {
            possible_ = true;
            for (auto iiti = iti->begin(), ati = alphabetTokens_.begin(); possible_ && iiti != iti->end(); ++iiti, ++ati) {
                possible_ = iiti->length() == ati->length();
            }

            possible_ = possible_ && mapLetters(*iti);
        }
    }

    return *this;
}
}

void U850::operator()() const
{
    int N;
    std::cin >> N;
    std::cin.get();
    std::cin.get();
    solution sol;
    while (N--) {
        std::cin >> sol;
        std::cout << sol();
        if (N != 0) {
            std::cout << std::endl;
        }
    }
}
