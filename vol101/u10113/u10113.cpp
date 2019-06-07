#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10113.h"

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
#include <functional>

#include "gcd.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10113 instance;
    instance();
}

namespace {

    typedef std::tuple<size_t, uint32_t, uint32_t> payload_t;
    typedef std::vector<std::vector<payload_t>> matrix_t;

    class solution_t {
    public:
        solution_t() : cmd_(), from_name_(), to_name_(), from_qty_(), to_qty_(), present_(), trans_(), exchange_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return cmd_ != '.';
        }

        solution_t& operator()();
        void populate();

        bool is_query() const
        {
            return cmd_ == '?';
        }

    private:
        char cmd_;
        std::string from_name_, to_name_;
        uint32_t from_qty_, to_qty_;
        bool present_;
        std::map<std::string, size_t> trans_;
        matrix_t exchange_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.cmd_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.present_)
        {
            out << sol.from_qty_ << ' ' << sol.from_name_ << " = " << sol.to_qty_ << ' ' << sol.to_name_ << std::endl;
        }
        else
        {
            out << "? " << sol.from_name_ << " = ? " << sol.to_name_ << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        char tmp;
        std::cin >> from_name_ >> tmp >> to_name_;
        present_ = false;

        if (trans_.find(from_name_) != trans_.end() && trans_.find(to_name_) != trans_.end())
        {
            std::vector<uint32_t> from(trans_.size()), to(trans_.size());
            std::function<void(uint32_t)> dfs = [&](uint32_t u)
            {
                for (auto t : exchange_[u])
                {
                    size_t v;
                    uint32_t from_rate, to_rate;
                    std::tie(v, from_rate, to_rate) = t;

                    if (from[v] == 0)
                    {
                        auto from_qty (from[u] * from_rate), to_qty (to[u] * to_rate);
                        auto k (math::gcd(from_qty, to_qty));
                        from_qty /= k;
                        to_qty /= k;
                        from[v] = from_qty;
                        to[v] = to_qty;
                        dfs(v);
                    }
                }
            };
            auto u(trans_[from_name_] - 1), v(trans_[to_name_] - 1);
            from[u] = to[u] = 1;
            dfs(u);

            if (from[v])
            {
                from_qty_ = from[v], to_qty_ = to[v];
                present_ = true;
            }
        }

        return *this;
    }

    void solution_t::populate()
    {
        char temp;
        std::cin >> from_qty_ >> from_name_ >> temp >> to_qty_ >> to_name_;

        if (trans_[from_name_] == 0)
        {
            trans_[from_name_] = trans_.size();
        }

        if (trans_[to_name_] == 0)
        {
            trans_[to_name_] = trans_.size();
        }

        if (exchange_.size() < trans_.size())
        {
            exchange_.resize(trans_.size());
        }

        auto k (math::gcd(from_qty_, to_qty_));
        exchange_[trans_[from_name_] - 1].emplace_back(trans_[to_name_] - 1, from_qty_ / k, to_qty_ / k);
        exchange_[trans_[to_name_] - 1].emplace_back(trans_[from_name_] - 1, to_qty_ / k, from_qty_ / k);
    }

}

void U10113::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        if (sol.is_query())
        {
            std::cout << sol() << std::endl;
        }
        else
        {
            sol.populate();
        }
    }
}
