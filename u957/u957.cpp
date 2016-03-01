#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u957.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>

U957::U957()
{
}

namespace
{
class solver
{
public:
    solver() : years_(0), popes_(0), max_(std::numeric_limits<int>::min()), start_(), end_()
    {
    }

    friend std::istream& operator >> (std::istream& in, solver& s)
    {
        if (in >> s.years_) {
            in >> s.popes_;
            solver::elections_.clear();
            solver::elections_.reserve(s.popes_);
            std::generate_n(std::back_inserter(solver::elections_), s.popes_, [&]() {
                int e;
                in >> e;
                return e;
            });
        }
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, solver& s)
    {
        out << s.max_ << " " << *(s.start_) << " " << *(s.end_);
        return out;
    }

    void solve()
    {
        max_ = std::numeric_limits<int>::min();
        for (auto it = solver::elections_.begin(); it != solver::elections_.end(); ++it) {
            auto istart = it;
            auto iend = std::upper_bound(it, solver::elections_.end(), *(it) + years_ - 1);
            if ((iend - istart) > max_) {
                max_ = (iend - istart);
                start_ = istart;
                end_ = iend - 1;
            }
        }
    }

private:
    int years_;
    int popes_;
    int64_t max_;
    std::vector<int>::iterator start_;
    std::vector<int>::iterator end_;
    static std::vector<int> elections_;
};

std::vector<int> solver::elections_;
}

extern "C" { 	UVA_API_EXPORT void __cdecl invoke(); } void __cdecl invoke() { 	U957 instance; 	instance(); }
void U957::operator()()
{
    solver s;
    while (std::cin >> s) {
        s.solve();
        std::cout << s << std::endl;
    }
}
