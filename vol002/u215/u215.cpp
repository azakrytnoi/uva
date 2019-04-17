#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u215.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U215 instance;
    instance();
}

namespace {

    enum class status_t
    {
        Undefined, Calculating, Caclulated
    };
    struct cell_t
    {
        std::string expression_;
        int64_t value_;
        status_t status_;

        cell_t() : expression_(), value_(), status_(status_t::Undefined) {}

        friend std::istream& operator>>(std::istream& in, cell_t& cell)
        {
            std::getline(in, cell.expression_);
            cell.value_ = 0;
            cell.status_ = status_t::Undefined;
            return in;
        }
    };

    typedef std::array < cell_t, ('9' - '0' + 1) > row_t;
    typedef std::array < row_t, ('T' - 'A' + 1) > spread_t;

    class solution_t {
        uint16_t n_rows_, n_cols_;
        bool broken_;
        spread_t spread_;
    public:
        solution_t() : n_rows_(), n_cols_(), broken_(), spread_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return n_rows_ != 0 && n_cols_ != 0;
        }

        solution_t& operator()();
    private:
        int64_t calculate(const size_t row, const size_t col, bool& broken);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (in >> sol.n_rows_ >> sol.n_cols_)
        {
            if (sol)
            {
                in.ignore();
                sol.broken_ = false;

                for (size_t row = 0; row < sol.n_rows_; row++)
                {
                    for (size_t col = 0; col < sol.n_cols_; col++)
                    {
                        in >> sol.spread_[row][col];
                    }
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (not sol.broken_)
        {
            out << " ";

            for (size_t col = 0; col < sol.n_cols_; col++)
            {
                out << std::setw(6) << col;
            }

            out << std::endl;

            for (size_t row = 0; row < sol.n_rows_; row++)
            {
                out << static_cast<char>('A' + row);

                for (size_t col = 0; col < sol.n_cols_; col++)
                {
                    out << std::setw(6) << sol.spread_[row][col].value_;
                }

                out << std::endl;
            }
        }
        else
        {
            for (size_t row = 0; row < sol.n_rows_; row++)
            {
                for (size_t col = 0; col < sol.n_cols_; col++)
                {
                    if (sol.spread_[row][col].status_ == status_t::Calculating)
                    {
                        out << static_cast<char>('A' + row) << col << ": " << sol.spread_[row][col].expression_ << std::endl;
                    }
                }
            }
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        for (size_t row = 0; row < n_rows_; row++)
        {
            for (size_t col = 0; col < n_cols_; col++)
            {
                bool broken(false);
                calculate (row, col, broken);
                broken_ |= broken;
            }
        }

        return *this;
    }

    int64_t solution_t::calculate(const size_t row, const size_t col, bool& broken)
    {
        switch (spread_[row][col].status_)
        {
        case status_t::Caclulated:
            return spread_[row][col].value_;
            break;

        case status_t::Calculating:
            broken = true;
            return 0;

        default:
        {
            spread_[row][col].status_ = status_t::Calculating;
            auto ch = spread_[row][col].expression_.begin();
            bool sign(true);

            while (ch != spread_[row][col].expression_.end() && *ch != ' ')
            {
                int64_t term(0);

                switch (*ch)
                {
                case '+':
                    sign = true;
                    ++ch;
                    break;

                case '-':
                    sign = false;
                    ++ch;
                    break;

                default:
                    if (std::isdigit(*ch))
                    {
                        do
                        {
                            term *= 10;
                            term += *ch - '0';
                        }
                        while (std::isdigit(*(++ch)));
                    }
                    else
                    {
                        size_t refRow = *(ch++) - 'A';
                        size_t refCol = *(ch++) - '0';
                        term = calculate(refRow, refCol, broken);

                        if (broken)
                        {
                            return 0;
                        }
                    }

                    break;
                }

                spread_[row][col].value_ += sign ? term : -term;
            }

            spread_[row][col].status_ = status_t::Caclulated;
            return spread_[row][col].value_;
        }
        break;
        }
    }

}

void U215::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
