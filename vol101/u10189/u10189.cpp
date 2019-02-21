#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10189.h"

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
    U10189 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : field_(), N_(0), M_(0) {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return N_ + M_ != 0;
        }

        solution& operator ()();

    private:
        std::vector<std::string> field_;
        int16_t N_, M_;

        void calculate (int16_t row, int16_t col);
        static uint32_t nField_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.N_ >> sol.M_;

        if (sol) {
            sol.field_.clear();
            sol.field_.reserve(sol.N_);
            std::string line;
            std::getline(in, line);
            std::generate_n(std::back_inserter(sol.field_), sol.N_, [&]() -> std::string {
                std::getline(in, line);
                return line; });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << "Field #" << sol.nField_ << ":" << std::endl;
        std::ostream_iterator<std::string> oit(out, "\n");
        std::copy(sol.field_.begin(), sol.field_.end(), oit);
        return out;
    }

    solution& solution::operator ()()
    {
        nField_++;

        for (int16_t row = 0; row < N_; row++) {
            for (int16_t col = 0; col < M_; col++) {
                if (field_[row][col] == '.') {
                    calculate(row, col);
                }
            }
        }

        return *this;
    }

    inline void solution::calculate(int16_t row, int16_t col)
    {
        static std::vector<std::pair<int16_t, int16_t>> deltas ({
            {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}
        });
        int8_t cnt = std::accumulate(deltas.begin(), deltas.end(), 0, [&](uint16_t prev, const std::pair<int16_t, int16_t>& coord) {
            return (coord.first + row >= 0 && coord.first + row < N_ && coord.second + col >= 0 && coord.second + col < M_ &&
                    field_[coord.first + row][coord.second + col] == '*') ? prev + 1 : prev;
        });

        field_[row][col] = cnt + '0';
    }

    uint32_t solution::nField_ = 0;

}

void U10189::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
