#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10000.h"

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
    U10000 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), graph_(), start_(), max_(), end_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not graph_.empty();
        }
        solution_t& operator()();

    private:
        size_t case_no_;
        std::vector<std::vector<bool>> graph_;
        size_t start_;
        int32_t max_;
        size_t end_;

        int32_t traceBack(size_t end, std::vector<int32_t>& path);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.graph_.clear();
        size_t n(0);

        if (in >> n && n != 0) {
            sol.graph_ = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));
            in >> sol.start_;
            --sol.start_;
            size_t p(0), q(0);

            while (in >> p >> q && p != 0 && q != 0) {
                sol.graph_[p - 1][q - 1] = true;
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Case " << sol.case_no_ << ": The longest path from " << (sol.start_ + 1) << " has length " << sol.max_ << ", finishing at " <<
            (sol.end_ + 1) << "." <<
            std::endl;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        max_ = std::numeric_limits<int32_t>::min(), end_ = start_;
        ++case_no_;
        std::vector<int32_t> path(graph_.size(), -1);
        path[start_] = 0;

        for (size_t idx = 0; idx < graph_.size(); ++idx) {
            if (idx != start_) {
                auto current (traceBack(idx, path));

                if (current > max_) {
                    max_ = current, end_ = idx;
                }
            }
        }

        return *this;
    }

    int32_t solution_t::traceBack(size_t end, std::vector<int32_t>& path)
    {
        if (path[end] == -1) {
            int32_t longest = 0;

            for (size_t i = 0; i < graph_.size(); ++i) {
                if (graph_[i][end]) {
                    int32_t length = traceBack(i, path) + 1;

                    if ((length > 1 || i == start_) && length > longest) {
                        longest = length;
                    }
                }
            }

            path[end] = longest;
        }

        return path[end];

    }

}

void U10000::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
