#include "u10066.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace {
class solver {
    std::vector<int> a_;
    std::vector<int> b_;
    int seq_;
    int solution_;

    static
    int lcs(const int* a, const int* b, const size_t M, const size_t N);

public:
    solver() : seq_(0) {
        a_.reserve(100);
        b_.reserve(100);
    }

    friend
    std::istream& operator >> (std::istream& in, solver& s) {
        int n, m;
        auto load = [&]() {
            int tmp;
            in >> tmp;
            return tmp;
        };
        if (in >> m >> n && !(m == 0 && n == 0)) {
            s.seq_++;
            s.a_.clear();
            std::generate_n(std::back_inserter(s.a_), m, load);
            s.b_.clear();
            std::generate_n(std::back_inserter(s.b_), n, load);
        } else {
            while (in >> n);
        }
        return in;
    }

    friend
    std::ostream& operator << (std::ostream& out, const solver& s) {
        out << "Twin Towers #" << s.seq_ << std::endl << "Number of Tiles : " << s.solution_ << std::endl;
        return out;
    }

    solver& solve() {
        solution_ = lcs(a_.data(), b_.data(), a_.size(), b_.size());
        return *this;
    }
};

int solver::lcs(const int* a, const int* b, const size_t M, const size_t N) {
#ifdef _WIN32
    int temp[11 + 1][110 + 1];
#else
	int temp[M + 1][N + 1];
#endif

    for (size_t i = 0; i <= M; i++) {
        for (size_t j = 0; j <= N; j++) {
            if (i == 0 || j == 0) {
                temp[i][j] = 0;
            }
            else if (a[i - 1] == b[j - 1]) {
                temp[i][j] = temp[i - 1][j - 1] + 1;
            }
            else {
                temp[i][j] = std::max(temp[i - 1][j], temp[i][j - 1]);
            }
        }
    }
    return temp[M][N];
}
}

void U10066::operator()()
{
    solver s;
    while (std::cin >> s) {
        std::cout << s.solve() << std::endl;
    }
}
