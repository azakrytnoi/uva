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

    struct cell_t;
    struct expression_t {
        enum class op_t : char {
            plus = '+', minus = '-', none = '\0'
        };
        enum class expr_type_t : uint8_t {
            none, ref, expr
        };

        expr_type_t type_;
        std::string str_val_;

        union val_t {
            struct {
                op_t op_;
                std::shared_ptr<expression_t> left_;
                std::shared_ptr<expression_t> right_;
            } a_;
            cell_t* ref_;
            val_t() {}
            ~val_t() {}
        } val_;

        expression_t() : type_(expr_type_t::none), str_val_(), val_()
        {
            val_.ref_ = nullptr;
        }
        ~expression_t() {}

    };

    struct cell_t {
        int64_t value_;
        std::shared_ptr<expression_t> expr_;
        bool evaluated_;

        cell_t () : value_(), expr_(), evaluated_(false) {}
        bool evaluate();

        friend std::istream& operator >> (std::istream& in, cell_t& cell)
        {
            char ch(0);
            cell.evaluated_ = false;

            while (in >> ch) {
                if (not cell.expr_ && std::isdigit(ch)) {
                    cell.value_ *= 10;
                    cell.value_ += ch - '0';
                    cell.evaluated_ = true;
                } else {
                    if (not cell.expr_) {
                        cell.expr_ = std::make_shared<expression_t>();
                    }

                    cell.expr_->str_val_.push_back(ch);
                }
            }

            return in;
        }
    };

    typedef std::array < cell_t, ('9' - '0') > row_t;
    typedef std::array < row_t, ('T' - 'A') > spread_t;

    class solution {
        uint16_t n_rows_, n_cols_;
        bool broken_;
    public:
        static spread_t spread_;
        solution() : n_rows_(), n_cols_(), broken_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return n_rows_ != 0 && n_cols_ != 0;
        }

        solution& operator()();
    private:
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        if (in >> sol.n_rows_ >> sol.n_cols_) {
            if (sol) {
                sol.broken_ = false;
                std::string line;
                std::getline(in, line);
                std::generate_n(sol.spread_.begin(), sol.n_rows_, [&]() {
                    row_t row;
                    std::generate_n(row.begin(), sol.n_cols_, [&]() {
                        cell_t cell;
                        std::getline(in, line);
                        std::stringstream line_stream(line);
                        line_stream >> cell;
                        return cell;
                    });
                    return row;
                });
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (not sol.broken_) {
            out << std::setw(2) << ' ';

            for (auto ch : "0123456789") {
                if ((ch - '0') >= sol.n_cols_) {
                    break;
                }

                out << std::setw(5) << std::right << ch;
            }

            out << std::endl;
            uint16_t n_rows(0);

            for (auto& row : sol.spread_) {
                if (n_rows >= sol.n_rows_) {
                    break;
                }

                out << std::setw(2) << std::left << static_cast<char>('A' + n_rows);
                uint16_t n_cols(0);

                for (auto& cell : row) {
                    if (n_cols >= sol.n_cols_) {
                        break;
                    }

                    out << std::setw(5) << std::right << cell.value_;
                    n_cols++;
                }

                out << std::endl;
                n_rows++;
            }
        } else {
            uint16_t n_rows(0);

            for (auto& row : sol.spread_) {
                if (n_rows >= sol.n_rows_) {
                    break;
                }

                uint16_t n_cols(0);

                for (auto& cell : row) {
                    if (n_cols >= sol.n_cols_) {
                        break;
                    }

                    if (not cell.evaluated_ && cell.expr_ != nullptr) {
                        out << static_cast<char>('A' + n_rows) << static_cast<char>('0' + n_cols) << ": " << cell.expr_->str_val_ << std::endl;
                    }

                    n_cols++;
                }

                n_rows++;
            }
        }

        return out;
    }

    solution& solution::operator()()
    {
        uint16_t n_rows(0);

        for (auto& row : spread_) {
            if (broken_ || ++n_rows >= n_rows_) {
                break;
            }

            uint16_t n_cols(0);

            for (auto& cell : row) {
                if (broken_ || ++n_cols >= n_cols_) {
                    break;
                }

                if (not cell.evaluated_) {
                    if (not cell.evaluate()) {
                        broken_ = true;
                        break;
                    }
                }
            }
        }

        return *this;
    }

    solution g_sol;

    std::shared_ptr<expression_t> term(std::string::iterator& str)
    {
        auto t = std::make_shared<expression_t>();
        uint16_t row = (*str++) - 'A';
        uint16_t col = (*str++) - '0';
        t->val_.ref_ = &g_sol.spread_ [row][col];
        t->type_ = expression_t::expr_type_t::ref;
        return t;
    }

    void parse(std::shared_ptr<expression_t>& root)
    {
        auto str = root->str_val_.begin();
        auto t = term(str);

        if (str != root->str_val_.end()) {
            root->val_.a_.left_ = t;
            root->val_.a_.op_ = static_cast<expression_t::op_t>(*str++);
            root->val_.a_.right_ = term(str);
            root->type_ = expression_t::expr_type_t::expr;
        } else {
            root->val_.ref_ = t->val_.ref_;
            root->type_ = expression_t::expr_type_t::ref;
        }
    }

    bool cell_t::evaluate()
    {
        if (expr_->type_ == expression_t::expr_type_t::none) {
            parse(expr_);
        }

        if (expr_->type_ == expression_t::expr_type_t::expr) {
            evaluated_ = true;

            if (not expr_->val_.a_.left_->val_.ref_->evaluated_) {
                expr_->val_.a_.left_->val_.ref_->evaluate();
            }

            if (not expr_->val_.a_.right_->val_.ref_->evaluated_) {
                expr_->val_.a_.right_->val_.ref_->evaluate();
            }

            switch (expr_->val_.a_.op_) {
            case expression_t::op_t::plus:
                value_ = expr_->val_.a_.left_->val_.ref_->value_ + expr_->val_.a_.right_->val_.ref_->value_;
                return true;

            case expression_t::op_t::minus:
                value_ = expr_->val_.a_.left_->val_.ref_->value_ - expr_->val_.a_.right_->val_.ref_->value_;
                return true;

            default:
                break;
            }
        } else if (expr_->type_ == expression_t::expr_type_t::ref) {
            if (expr_->val_.ref_ == this) {
                return false;
            }

            value_ = expr_->val_.ref_->value_;
            evaluated_ = true;
            return true;
        }

        return false;
    }

    spread_t solution::spread_;
}

void U215::operator()() const
{

    while (std::cin >> g_sol && g_sol) {
        std::cout << g_sol() << std::endl;
    }
}
