#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10010.h"

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
    U10010 instance;
    instance();
}

namespace {
class solution {
    std::vector<std::string> grid_;
    int nCols_, nRows_;
    static const std::vector<std::pair<int, int>> deltas_;
public:
    solution() :
        grid_(), nCols_(0), nRows_(0)
    {}

    friend std::istream& operator >>(std::istream& in, solution& sol);

    std::pair<size_t, size_t> solve(const std::string& word) const;

private:
    bool lookup(size_t row, size_t col, const std::string& word) const;
};

std::istream& operator >>(std::istream& in, solution& sol)
{
    sol.grid_.clear();
    std::string line;
    in >> sol.nRows_ >> sol.nCols_;
    sol.grid_.reserve(sol.nRows_);
    std::generate_n(std::back_inserter(sol.grid_), sol.nRows_, [&]() {
        in >> line;
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        return line;
    });
    return in;
}

std::ostream& operator <<(std::ostream& out, const std::pair<size_t, size_t>& p)
{
    out << p.first << " " << p.second;
    return out;
}

const std::vector<std::pair<int, int>> solution::deltas_ = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

bool solution::lookup(size_t row, size_t col, const std::string& word) const
{
    if (grid_[row][col] == word[0]) {
        for (auto delta = deltas_.begin(); delta != deltas_.end(); ++delta) {
            int64_t lookup_row(row + delta->first), lookup_col(col + delta->second);
            size_t matched_pos(1);

            for (;
                    matched_pos < word.length()
                    && !(lookup_row >= nRows_ || lookup_row < 0 || lookup_col >= nCols_ || lookup_col < 0)
                    && grid_[lookup_row][lookup_col] == word[matched_pos];
                    matched_pos++,
                    lookup_row += delta->first,
                    lookup_col += delta->second) {
            }

            if (matched_pos == word.length()) {
                return true;
            }
        }
    }

    return false;
}

std::pair<size_t, size_t> solution::solve(const std::string& word) const
{
    for (size_t row = 0; row < size_t(nRows_); row++) {
        for (size_t col = 0; col < size_t(nCols_); col++) {
            if (lookup(row, col, word)) {
                return std::make_pair(row + 1, col + 1);
            }
        }
    }

    return std::make_pair(std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max());
}
} // namespace

void U10010::operator()() const
{
    solution sol;
    int N;
    std::cin >> N;

    while (N--) {
        std::cin >> sol;
        int K;
        std::cin >> K;

        while (K--) {
            std::string word;
            std::cin >> word;
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            std::cout << sol.solve(word) << std::endl;
        }

        std::cout << std::endl;
    }
}
