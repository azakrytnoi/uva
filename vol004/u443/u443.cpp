#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u443.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <map>
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
    U443 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_(), numbers_()
        {
            numbers_.reserve(max_);
            numbers_.push_back(0);
            std::priority_queue<uint64_t, std::deque<uint64_t>, std::greater<uint64_t>> work;
            work.push(1);

            std::generate_n(std::back_inserter(numbers_), max_, [&]()
            {
                auto num = work.top();
                work.pop();

                while (not work.empty() && work.top() == num)
                {
                    work.pop();
                }

                work.push(num * 2);
                work.push(num * 3);
                work.push(num * 5);
                work.push(num * 7);

                return num;
            });
        }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return n_ != 0;
        }
        solution_t& operator()();

    private:
        size_t n_;
        const size_t max_ = 5845;
        std::vector<uint64_t> numbers_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        auto suffix = [](size_t n)
        {
            n %= 100;

            if (n > 10 && n < 20)
            {
                return "th";
            }

            switch (n % 10)
            {
            case 1:
                return "st";

            case 2:
                return "nd";

            case 3:
                return "rd";

            default:
                return "th";
            }
        };
        out << "The " << sol.n_ << suffix(sol.n_) << " humble number is " << sol.numbers_[sol.n_] << ".";
        return out;
    }

    solution_t& solution_t::operator()()
    {
        return *this;
    }

}

void U443::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
