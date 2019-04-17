#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u442.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U442 instance;
    instance();
}

namespace {

    struct matrix_t
    {
        char name_;
        size_t row_, col_;

        matrix_t(char name = 0, size_t row = 0, size_t col = 0) : name_(name), row_(row), col_(col) {}

        friend std::istream& operator>>(std::istream& in, matrix_t& mat)
        {
            in >> mat.name_ >> mat.row_ >> mat.col_;
            return in;
        }
    };

    class solution_t {
    public:
        solution_t() : matrices_(), expr_(), count_(), valid_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not expr_.empty();
        }
        solution_t& operator()();

    private:
        std::map<char, std::shared_ptr<matrix_t>> matrices_;
        std::string expr_;
        size_t count_;
        bool valid_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.matrices_.empty())
        {
            size_t n(0);
            in >> n;

            while (n--)
            {
                auto mat (std::make_shared<matrix_t>());
                in >> *mat;
                sol.matrices_.insert(std::make_pair(mat->name_, mat));
            }
        }

        in >> sol.expr_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.valid_)
        {
            out << sol.count_;
        }
        else
        {
            out << "error";
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        count_ = 0;
        valid_ = true;
        std::stack<std::shared_ptr<matrix_t>> work;

        for (auto ch : expr_)
        {
            switch (ch)
            {
            case ')':
            {
                auto rhs (work.top());
                work.pop();
                auto lhs (work.top());
                work.pop();

                if (lhs->col_ == rhs->row_)
                {
                    count_ += lhs->row_ * lhs->col_ * rhs->col_;
                    auto inter (std::make_shared<matrix_t>('x', lhs->row_, rhs->col_));
                    work.push(inter);
                }
                else
                {
                    valid_ = false;
                    break;
                }
            }
            break;

            case '(':
                break;

            default:
                work.push(matrices_.find(ch)->second);
                break;
            }
        }

        return *this;
    }

}

void U442::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
