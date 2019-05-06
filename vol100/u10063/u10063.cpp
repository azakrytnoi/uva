#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10063.h"

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

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10063 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : src_(), tmp_(), ans_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        std::string src_;
        std::string tmp_;
        std::vector<std::string> ans_;

        void dfs(size_t idx);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.src_;
        sol.ans_.clear();
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<std::string> sout (out, "\n");
        std::copy(sol.ans_.begin(), sol.ans_.end(), sout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        tmp_.clear();
        tmp_.resize(src_.size());
        dfs(0);
        return *this;
    }

    void solution_t::dfs(size_t idx)
    {
        if (idx == src_.size())
        {
            ans_.push_back(tmp_);
            return;
        }

        for (auto i = idx; i >= 1; --i)
        {
            tmp_[i] = tmp_[i - 1];
        }

        for (auto i = 0u; i <= idx; ++i)
        {
            tmp_[i] = src_[idx];
            dfs(idx + 1);
            tmp_[i] = tmp_[i + 1];
        }
    }

}

void U10063::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
