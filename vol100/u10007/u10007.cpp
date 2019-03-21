#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10007.h"

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
    U10007 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : factorial_(), n_(), result_() {
        	factorial_.reserve(601);
        	factorial_.push_back(1);
        	factorial_.push_back(1);
        	math::uint_big_t current (factorial_.back());
        	std::generate_n(std::back_inserter(factorial_), 600, [&](){
        		current *= math::uint_big_t(factorial_.size());
        		return current;
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
        std::vector<math::uint_big_t> factorial_;
        size_t n_;
        math::uint_big_t result_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
    	in >> sol.n_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
    	out << sol.result_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
    	result_ = factorial_[n_ * 2] / factorial_[n_ + 1];
        return *this;
    }

}

void U10007::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
