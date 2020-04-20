#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u13282.h"

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
    U13282 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : in_times_(), out_times_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return hasData_;
        }
        solution_t& operator()();

    private:
        size_t N_{0}, M_{0};
        std::vector<uint32_t> in_times_;
        std::vector<uint32_t> out_times_;
        uint32_t result_{0};
        bool hasData_{false};
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.hasData_ = (bool)(in >> sol.N_ >> sol.M_);

        if (sol.hasData_)
        {
            sol.in_times_.clear();
            sol.in_times_.reserve(sol.N_);
            sol.out_times_.clear();
            sol.out_times_.reserve(sol.M_);
            std::generate_n(std::back_inserter(sol.in_times_), sol.N_, [&]() { uint32_t tmp; in >> tmp; return tmp; });
            std::generate_n(std::back_inserter(sol.out_times_), sol.M_, [&]() { uint32_t tmp; in >> tmp; return tmp; });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.result_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::map<uint32_t, uint32_t> distribution;

        for (size_t i = 0, possible_time = 0; i < N_; i++)
        {
            while (possible_time < M_ && out_times_[possible_time] <= in_times_[i])
            {
                possible_time++;
            }

            auto in_at = in_times_[i];

            for (size_t j = possible_time; j < M_; j++)
            {
                distribution[out_times_[j] - in_at]++;
            }
        }

        result_ = std::numeric_limits<uint32_t>::max();
        uint32_t current = 0;

        for (auto [timeDelta, freq] : distribution)
        {
            if (freq > current)
            {
                current = freq;
                result_ = std::numeric_limits<uint32_t>::max();
            }

            if (freq == current)
            {
                result_ = std::min(result_, timeDelta);
            }
        }

        return *this;
    }

}

void U13282::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
