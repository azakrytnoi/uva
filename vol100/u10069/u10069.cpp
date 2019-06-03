#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10069.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

#include "biguint.h"


extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10069 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), main_(), sub_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::string main_, sub_;
        math::uint_big_t result_;

        typedef std::vector<std::vector<math::uint_big_t>> matrix_t;
        math::uint_big_t count(size_t main_pos, size_t sub_pos, matrix_t& matrix);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        in >> sol.main_ >> sol.sub_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.result_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        matrix_t mat(std::vector<std::vector<math::uint_big_t>>(main_.size() + 1, std::vector<math::uint_big_t>(sub_.size() + 1)));
        result_ = count(0u, 0u, mat);
        return *this;
    }

    math::uint_big_t solution_t::count(size_t main_pos, size_t sub_pos, matrix_t& matrix)
    {
        if (sub_pos == sub_.length())
        {
            return 1;
        }

        if (main_pos == main_.length())
        {
            return 0;
        }

        if (matrix[main_pos][sub_pos] == 0)
        {
            auto cnt(count(main_pos + 1, sub_pos, matrix));

            if (main_[main_pos] == sub_[sub_pos])
            {
                cnt += count(main_pos + 1, sub_pos + 1, matrix);
            }

            matrix[main_pos][sub_pos] = cnt;
        }

        return matrix[main_pos][sub_pos];
    }
}

void U10069::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
