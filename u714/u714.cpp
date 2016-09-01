#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u714.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <numeric>

U714::U714()
{
}

namespace
{
class solver
{
public:
    solver(int books, int scribers) : books_(books), scribers_(scribers), optimal_(0)
    {
        pages_.clear();
        pages_.reserve(books_);
    }

    inline void solve()
    {
        int64_t sum(std::accumulate(pages_.begin(), pages_.end(), 0ll));
        int64_t lmin(*(std::minmax_element(pages_.begin(), pages_.end()).first));
        optimal_ = binary(lmin, sum);
    }

    friend std::istream& operator >> (std::istream& in, solver& s)
    {
        std::string line;
        std::getline(in, line);
        std::getline(in, line);
        std::stringstream ss(line);
        std::istream_iterator<int> iin(ss);
        std::copy(iin, std::istream_iterator<int>(), std::back_inserter(s.pages_));
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, solver& s)
    {
        s.output(out);
        return out;
    }

private:
    static std::vector<int64_t> pages_;
    static std::vector<bool> breaks_;
    const int books_;
    const int scribers_;
    int64_t optimal_;

    inline int divide(int64_t M)
    {
        breaks_.clear();
        breaks_.resize(books_);
        int cnt = 0;
        int pos = books_ - 1;
        while (pos >= 0) {
            int64_t sum = 0;
            bool ok = true;
            while (pos >= 0 && sum + pages_[pos] <= M) {
                ok = false;
                sum += pages_[pos];
                --pos;
            }
            if (ok) {
                return scribers_ + 1;
            }
            if (pos >= 0) {
                breaks_[pos] = true;
            }
            ++cnt;
        }
        return cnt;
    }

    inline int64_t binary(int64_t lmin, int64_t sum)
    {
        int64_t left = lmin, right = sum;
        while (left < right) {
            int64_t mid = (left + right) >> 1;
            if (divide(mid) <= scribers_) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return right;
    }

    inline void output(std::ostream& out)
    {
        int cnt = divide(optimal_);
        for (int i = 0; i < books_ - 1 && cnt < scribers_; ++i) {
            if (!breaks_[i]) {
                breaks_[i] = true;
                ++cnt;
            }
        }
        for (int i = 0; i < books_; ++i) {
            if (i) {
                out << " " << pages_[i];
            } else {
                out << pages_[i];
            }
            if (breaks_[i]) {
                out << " /";
            }
        }
        out << std::endl;
    }
};

std::vector<int64_t> solver::pages_;
std::vector<bool> solver::breaks_;
}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U714 instance;
    instance();
}
void U714::operator()() const
{
    int T;
    std::cin >> T;
    while (T--) {
        int books, scribers;
        std::cin >> books >> scribers;
        solver s(books, scribers);
        std::cin >> s;
        s.solve();
        std::cout << s;
    }
}
