#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u402.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
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
	U402 instance;
	instance();
}

namespace
{

class solution_t
{
public:
    solution_t() : case_no_(), lucky_(), desk_(), ppl_() {}

    operator bool() { return not desk_.empty(); }

    solution_t& operator()();

    friend std::istream& operator>>(std::istream& in, solution_t& sol);

    friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

private:
    size_t case_no_;
    size_t lucky_;
    std::vector<size_t> desk_;
    std::list<size_t> ppl_;
};

solution_t& solution_t::operator()()
{
    case_no_++;

    for (auto card = desk_.begin(); card != desk_.end() && ppl_.size() > lucky_; ++card)
    {
        size_t cnt(0);

        for (auto ppl = ppl_.begin(); ppl != ppl_.end() && ppl_.size() > lucky_;)
        {
            if (++cnt == *card)
            {
                cnt = 0;
                ppl = ppl_.erase(ppl);
            }
            else
            {
                ++ppl;
            }
        }
    }

    return *this;
}

std::istream& operator>>(std::istream& in, solution_t& sol)
{
    sol.desk_.clear();
    sol.ppl_.clear();

    if (in >> sol.lucky_)
    {
        std::generate_n(std::back_inserter(sol.ppl_), sol.lucky_, [&]() { return sol.ppl_.size() + 1; });
        sol.desk_.reserve(20);
        in >> sol.lucky_;
        std::istream_iterator<size_t> din(in);
        std::copy_n(din, sol.desk_.capacity(), std::back_inserter(sol.desk_));
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const solution_t& sol)
{
    out << "Selection #" << sol.case_no_ << std::endl;
    std::ostream_iterator<size_t> pout(out, " ");
    std::copy(sol.ppl_.begin(), sol.ppl_.end(), pout);
    out << std::endl;
    return out;
}


}

void U402::operator()() const
{
    solution_t sol;
    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
