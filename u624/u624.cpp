#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u624.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <bitset>
#include <algorithm>
#include <numeric>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U624 instance;
    instance();
}

U624::U624()
{}

U624::~U624()
{}

namespace {

template<size_t N>
class solver {
    std::vector<int> tracks_;
    std::bitset<N> solution_;
    std::bitset<N> current_;
    int64_t total_, capacity_;

public:
    solver() :
        tracks_(), solution_(), current_(), total_(0), capacity_(0)
    {
        tracks_.reserve(N);
    }

    friend std::istream& operator >>(std::istream& in, solver& s)
    {
        in >> s.capacity_;
        int nTracks;
        in >> nTracks;
        s.tracks_.clear();
        std::istream_iterator<int> iin(in);
        std::copy(iin, std::istream_iterator<int>(), std::back_inserter(s.tracks_));
        s.total_ = 0;
        return in;
    }

    friend std::ostream& operator <<(std::ostream& out, const solver& s)
    {
        std::ostream_iterator<int> oit(out, " ");
        size_t idx(0);
        std::copy_if(s.tracks_.begin(), s.tracks_.end(), oit, [&](auto /*track*/) -> bool {return s.solution_[idx++]; });
        out << "sum:" << s.total_;
        return out;
    }

    void solve(size_t level = 0)
    {
        if (level == tracks_.size()) {
            size_t idx(0);
            int64_t running = std::accumulate(tracks_.begin(), tracks_.end(), int64_t(0),
                                              [&](int64_t init, auto val) -> int64_t {return init + (current_[idx++] ? val : 0); });

            if (running <= capacity_ && running > total_) {
                total_ = running;
                solution_ = current_;
            }

            return;
        }

        current_[level] = true;
        solve(level + 1);
        current_[level] = false;
        solve(level + 1);
    }
};
}

void U624::operator()() const
{
    std::string line;
    solver<20> s;

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        ss >> s;
        s.solve();
        std::cout << s << std::endl;
    }
}
