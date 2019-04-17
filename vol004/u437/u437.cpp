#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u437.h"

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
    U437 instance;
    instance();
}

namespace {

    struct block_t
    {
        int32_t x_, y_, z_;

        block_t() : x_(), y_(), z_() {}

        friend bool operator >(const block_t& lhs, const block_t& rhs)
        {
            return (lhs.x_ > rhs.x_ && lhs.y_ > rhs.y_) || (lhs.x_ > rhs.y_ && lhs.y_ > rhs.x_);
        }

        friend std::istream& operator >>(std::istream& in, block_t& block)
        {
            in >> block.x_ >> block.y_ >> block.z_;
            return in;
        }
    };

    class solution_t {
    public:
        solution_t() : case_no_(), blocks_(), max_() {}

        operator bool()
        {
            return not blocks_.empty();
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t case_no_;
        std::vector<block_t> blocks_;
        size_t max_;
    };

    solution_t& solution_t::operator()()
    {
        ++case_no_;
        size_t n_blocks = blocks_.size();
        blocks_.resize(n_blocks * 3);
        auto orient = [](block_t& block)
        {
            if (block.y_ > block.x_)
            {
                std::swap(block.x_, block.y_);
            }
        };

        for (size_t i = 0; i < n_blocks; ++i)
        {
            blocks_[n_blocks + i] = blocks_[n_blocks * 2 + i] = blocks_[i];
            orient(blocks_[i]);
            std::swap(blocks_[n_blocks + i].x_, blocks_[n_blocks + i].z_);
            orient(blocks_[n_blocks + i]);
            std::swap(blocks_[n_blocks * 2 + i].y_, blocks_[n_blocks * 2 + i].z_);
            orient(blocks_[n_blocks * 2 + i]);
        }

        std::sort(blocks_.begin(), blocks_.end(), [](const block_t& lhs, const block_t& rhs)
        {
            return lhs.x_ > rhs.x_;
        });
        std::vector<size_t> dp(blocks_.size());
        std::transform(blocks_.begin(), blocks_.end(), dp.begin(), [](const block_t& block)
        {
            return block.z_;
        });

        auto di = dp.begin();

        for (auto i = blocks_.begin(); i != blocks_.end(); ++i, ++di)
        {
            auto dj = dp.begin();

            for (auto j = blocks_.begin(); j != blocks_.end(); ++j, ++dj)
            {
                if (i != j && *i > *j)
                {
                    *dj = std::max(*dj, *di + j->z_);
                }
            }
        }

        max_ = std::accumulate(dp.begin(), dp.end(), 0, [](size_t prev, size_t height)
        {
            return std::max(prev, height);
        });
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        size_t n(0);
        sol.blocks_.clear();

        if (in >> n && n > 0)
        {
            sol.blocks_.reserve(n);
            std::istream_iterator<block_t> bin(in);
            std::copy_n(bin, n, std::back_inserter(sol.blocks_));
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << "Case " << sol.case_no_ << ": maximum height = " << sol.max_;
        return out;
    }

}

void U437::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
