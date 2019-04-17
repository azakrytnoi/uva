#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u441.h"

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
    U441 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : set_(), selected_(6) { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not set_.empty();
        }
        solution_t& operator()(std::ostream& out);

    private:
        std::vector<uint16_t> set_;
        std::vector<uint16_t> selected_;

        void select_and_print(size_t selected, size_t pos, std::ostream& out);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.set_.clear();
        size_t n;

        if (in >> n && n > 0)
        {
            sol.set_.reserve(n);
            std::istream_iterator<uint16_t> sin(in);
            std::copy_n(sin, n, std::back_inserter(sol.set_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& /*sol*/)
    {
        return out;
    }

    solution_t& solution_t::operator()(std::ostream& out)
    {
        std::sort(set_.begin(), set_.end());
        select_and_print(0, 0, out);
        return *this;
    }

    void solution_t::select_and_print(size_t selected, size_t pos, std::ostream& out)
    {
        if (selected + set_.size() - pos < 6)
        {
            return;
        }

        if (pos == set_.size())
        {
            if (selected == 6)
            {
                auto sel = selected_.begin();
                out << *(sel++);
                std::for_each(sel, selected_.end(), [&](const uint16_t num)
                {
                    out << ' ' << num;
                });
                out << std::endl;
            }

            return;
        }

        if (selected < 6)
        {
            selected_[selected] = set_[pos];
            select_and_print(selected + 1, pos + 1, out);
        }

        select_and_print(selected, pos + 1, out);
    }

}

void U441::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol(std::cout) << std::endl;
    }
}
