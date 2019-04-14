#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10020.h"

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
    U10020 instance;
    instance();
}

namespace {

    struct pair_t {
        int32_t start_, end_;
        pair_t(int32_t start = 0, int32_t end = 0) : start_(start), end_(end) {}

        bool operator < (const pair_t& rhs)
        {
            return start_ < rhs.start_;
        }

        operator bool () const
        {
            return start_ != 0 || end_ != 0;
        }

        friend std::istream& operator >>(std::istream& in, pair_t& pair)
        {
            in >> pair.start_ >> pair.end_;
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const pair_t& pair)
        {
            out << pair.start_ << ' ' << pair.end_;
            return out;
        }
    };


    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), M_(), pairs_(), lastX_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        int32_t M_;
        std::vector<pair_t> pairs_;
        int32_t lastX_;
        std::vector<pair_t> result_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max()) {
            in >> sol.N_;
        }

        in >> sol.M_;
        sol.pairs_.clear();
        pair_t pair;

        while (in >> pair && pair) {
            if (pair.start_ < sol.M_ && pair.end_ > 0) {
                sol.pairs_.push_back(pair);
            }
        }

        sol.pairs_.shrink_to_fit();
        sol.pairs_.reserve(sol.pairs_.size() + 1);
        sol.result_.clear();
        sol.result_.resize(1);
        sol.result_.reserve(sol.pairs_.capacity());
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.lastX_ > 0) {
            out << sol.result_.size() - 1 << std::endl;
            std::ostream_iterator<pair_t> pout(out, "\n");
            std::copy_n(sol.result_.begin(), sol.result_.size() - 1, pout);
        } else {
            out << 0 << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::sort(pairs_.begin(), pairs_.end());
        pairs_.push_back(M_ + 1);
        lastX_ = 0;
        auto currentPair = pairs_.begin();

        while (lastX_ < M_ && result_.size() < pairs_.size()) {
            result_.back().end_ = 0;

            for (; currentPair->start_ <= lastX_; ++currentPair)
                if (currentPair->end_ > result_.back().end_) {
                    result_.back() = *currentPair;
                }

            if (result_.back().end_ == lastX_) {
                break;
            }

            lastX_ = result_.back().end_;
            result_.push_back(0);
        }

        if (lastX_ < M_) {
            lastX_ = 0;
        }

        return *this;
    }

}

void U10020::operator()() const
{
    solution_t sol;

    std::string separator;

    while (std::cin >> sol && sol) {
        std::cout << separator << sol();
        separator = "\n";
    }
}
