#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u208.h"

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
    U208 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), corner_(), visited_(), map_(), path_()
        {
            visited_.resize(CORNERS);
            map_.resize(CORNERS, std::vector<bool>(CORNERS));
        }

        operator bool()
        {
            return corner_ != 0;
        }

        solution_t& operator()()
        {
            case_no_++;
            visited_[1] = true;
            path_.reserve(CORNERS);
            dfs(1, 0);
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            sol.path_.clear();
            sol.corner_ = 0;

            if (in >> sol.corner_) {
                std::fill(sol.visited_.begin(), sol.visited_.end(), false);
                std::for_each(sol.map_.begin(), sol.map_.end(), [](std::vector<bool>& row) {
                    std::fill(row.begin(), row.end(), false);
                });
                uint16_t x(0), y(0);

                while (in >> x >> y && not(x == 0 && y == 0)) {
                    sol.map_[x][y] = sol.map_[y][x] = true;
                }
            }

            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << "CASE " << sol.case_no_ << std::endl;
            std::for_each(sol.path_.begin(), sol.path_.end(), [&](const std::vector<uint16_t>& path) {
                out << "1";
                std::for_each(path.begin(), path.end(), [&](const uint16_t i) {
                    out << " " << i;
                });
                out << std::endl;
            });
            out << "There are " << sol.path_.size() << " routes from the firestation to streetcorner " << sol.corner_ << ".";
            return out;
        }

    private:
        size_t case_no_, corner_;
        std::vector<bool> visited_;
        std::vector<std::vector<bool>> map_;
        std::vector<std::vector<uint16_t>> path_;

        const size_t CORNERS = 23;

        void dfs(uint16_t node, uint16_t dep)
        {
            static std::vector<uint16_t> path(CORNERS);

            if (node == corner_) {
                path_.push_back(std::vector<uint16_t>());
                path_.back().resize(dep);
                std::copy_n(path.begin(), dep, path_.back().begin());
                return;
            }

            for (size_t idx = 1; idx <= 21; idx++) {
                if (map_[node][idx] && not visited_[idx]) {
                    if (not reach_corner(idx)) {
                        continue;
                    }

                    visited_[idx] = true;
                    path[dep] = idx;
                    dfs(idx, dep + 1);
                    visited_[idx] = false;
                }
            }
        }

        bool reach_corner(uint16_t startnode)
        {
            std::vector<uint16_t> cornersQ(CORNERS);
            std::vector<bool> used(CORNERS);
            size_t Qidx(0), targetnode;
            cornersQ[Qidx] = startnode;

            for (uint16_t i = 0; i <= Qidx; i++) {
                targetnode = cornersQ[i];

                if (targetnode == corner_) {
                    return true;
                }

                for (uint16_t j = 1; j <= 21; j++) {
                    if (not visited_[j] && map_[targetnode][j] && not used[j]) {
                        used[j] = true;
                        cornersQ[++Qidx] = j;
                    }
                }
            }

            return false;
        }
    };

}

void U208::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
