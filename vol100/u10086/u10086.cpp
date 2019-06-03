#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10086.h"

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
#include <stack>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10086 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : T1_(), T2_(), ncpc_(), bcew_(), min_(), trace_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return T1_ > 0 && T2_ > 0;
        }
        solution_t& operator()();

    private:
        size_t T1_, T2_;
        typedef std::vector<std::vector<int64_t>> matrix_t;
        matrix_t ncpc_, bcew_;
        int64_t min_;
        std::stack<int64_t> trace_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (in >> sol.T1_ >> sol.T2_ && sol.T1_ > 0 && sol.T2_ > 0)
        {
            size_t n(0);
            in >> n;
            sol.ncpc_.clear();
            sol.ncpc_.resize(n);
            sol.bcew_.clear();
            sol.bcew_.resize(n);
            auto ncpc = sol.ncpc_.begin();
            auto bcew = sol.bcew_.begin();
            auto read_item = [&]() {
                int64_t item;
                in >> item;
                return item;
            };

            while (n-- > 0)
            {
                size_t m(0);
                in >> m;
                ncpc->reserve(m);
                bcew->reserve(m);
                std::generate_n(std::back_inserter(*ncpc), m, read_item);
                ++ncpc;
                std::generate_n(std::back_inserter(*bcew), m, read_item);
                ++bcew;
            }

        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.min_ << std::endl;
        std::stack temp(sol.trace_);
        std::ostream_iterator<int64_t> iout (out, " ");

        while (not temp.empty())
        {
            *(iout++) = temp.top();
            temp.pop();
        }

        out << std::endl;

        return out;
    }

    solution_t& solution_t::operator()()
    {
        matrix_t arr(ncpc_.size(), std::vector<int64_t> (T1_ + 1, std::numeric_limits<int64_t>::max()));
        matrix_t used(ncpc_.size(), std::vector<int64_t> (T1_ + 1, -1));

        for (auto i = 0u; i < T1_ + 1; ++i)
        {
            if (i > ncpc_[0].size())
            {
                break;
            }

            auto value (0);

            if (i == 0)
            {
                value = bcew_[0].back();
            }
            else if (i == ncpc_[0].size())
            {
                value = ncpc_[0].back();
            }
            else
            {
                value = ncpc_[0][i - 1] + bcew_[0][ncpc_[0].size() - i - 1];
            }

            arr[0][i] = value;
        }

        for (auto i = 1u; i < ncpc_.size(); ++i)
        {
            for (auto j = 0u; j < T1_ + 1; ++j)
            {
                if (arr[i - 1][j] != std::numeric_limits<int64_t>::max())
                {
                    for (auto k = 0u; k < T1_ - j + 1; ++k)
                    {
                        if (k > ncpc_[i].size())
                        {
                            break;
                        }

                        auto value = arr[i - 1][j];

                        if (k == 0)
                        {
                            value += bcew_[i].back();
                        }
                        else if (k == ncpc_[i].size())
                        {
                            value += ncpc_[i].back();
                        }
                        else
                        {
                            value += ncpc_[i][k - 1] + bcew_[i][ncpc_[i].size() - k - 1];
                        }

                        if (arr[i][j + k] > value)
                        {
                            arr[i][j + k] = value;
                            used[i][j + k] = k;
                        }

                    }
                }
            }
        }

        min_ = arr[ncpc_.size() - 1][T1_];

        while (not trace_.empty())
        {
            trace_.pop();
        }

        auto x = ncpc_.size() - 1;
        auto y = T1_;

        while (x > 0)
        {
            trace_.push(used[x][y]);
            y -= used[x][y];
            x -= 1;
        }

        trace_.push(y);
        return *this;
    }

}

void U10086::operator()() const
{
    solution_t sol;

    while (true)
    {
        std::cin >> sol;

        if (sol)
        {
            std::cout << sol() << std::endl;
        }
        else
        {
            break;
        }
    }
}
