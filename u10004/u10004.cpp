#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10004.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10004 instance;
    instance();
}

namespace {
class solution {
    int32_t n_, l_;
    bool bicolorable_;
    std::vector<std::vector<int32_t>> ed_;
public:
    solution() : n_(), l_(), bicolorable_(), ed_()
    {
        ed_.reserve(200);
    }

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const
    {
        return n_ != 0;;
    }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    in >> sol.n_ >> sol.l_;

    if (in) {
        sol.ed_.clear();
        sol.ed_.resize(sol.n_);

        for (int32_t i = 0; i < sol.l_; i++) {
            int32_t x, y;
            in >> x >> y;
            sol.ed_[x].push_back(y);
            sol.ed_[y].push_back(x);
        }
    }

    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << (sol.bicolorable_ ? "" : "NOT ") << "BICOLORABLE.";
    return out;
}

solution& solution::operator()()
{
    bicolorable_ = true;
    std::queue<int32_t> work;
    std::vector<int32_t> colors(n_, -1);
    colors[0] = 0;
    work.push(0);

    while (!work.empty()) {
        int32_t p = work.front();
        work.pop();

        for (size_t i = 0; i < ed_[p].size(); i++) {
            if (colors[ed_[p][i]] == -1) {
                colors[ed_[p][i]] = (colors[p] == 0 ? 1 : 0);
                work.push(ed_[p][i]);
            } else if (colors[ed_[p][i]] == colors[p]) {
                bicolorable_ = false;
                break;
            }
        }

        if (!bicolorable_) {
            break;
        }
    }

    return *this;
}
}

void U10004::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
