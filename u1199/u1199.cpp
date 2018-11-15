#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u1199.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <bitset>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U1199 instance;
    instance();
}

namespace {
template<size_t N>
class solver {
    int nFloors_;
    int max_;
    int upmost_;
    std::vector<int> stops_;
    std::bitset<N> floors_;

public:
    solver(int nFloors) : nFloors_(nFloors), max_(0), upmost_(0), stops_(), floors_()
    {
        stops_.reserve(N);
    }

    friend
    std::istream& operator >> (std::istream& in, solver& sol)
    {
        sol.upmost_ = -1;
        sol.stops_.clear();
        sol.floors_.reset();

        while (sol.nFloors_--) {
            in >> sol.upmost_;
            sol.floors_[sol.upmost_] = true;
        }

        return in;
    }

    friend
    std::ostream& operator << (std::ostream& out, const solver& sol)
    {
        out << sol.max_ << std::endl;
        out << sol.stops_.size();
        std::for_each(sol.stops_.begin(), sol.stops_.end(), [&](auto floor) {
            out << " " << floor;
        });
        return out;
    }

    void solve()
    {
        int min(0);
        max_ = 14 * (upmost_ - 1);

        while (min < max_ - 1) {
            int mid = (min + max_) / 2;

            if (solve(mid)) {
                max_ = mid;

            } else {
                min = mid;
            }
        }
    }

private:
    bool solve(int top)
    {
        stops_.clear();
        int num_stops(0);
        int next_floor(top / 20 + 2);

        while (next_floor <= upmost_) {
            while (next_floor <= upmost_ && floors_[next_floor] == false) {
                next_floor++;
            }

            stops_.push_back(next_floor);

            if ((next_floor - 1) * 4 + 10 * num_stops > top) {
                return false;
            }

            int temp = (top - 10 * num_stops + 20 * next_floor + 4) / 24;
            next_floor = (top - 10 * num_stops + 16 * temp + 4) / 20 + 1;
            num_stops++;
        }

        return true;
    }
};
}

void U1199::operator()() const
{
    int N;

    while ((std::cin >> N) && N) {
        solver<32> s(N);
        std::cin >> s;
        s.solve();
        std::cout << s << std::endl;
    }
}
