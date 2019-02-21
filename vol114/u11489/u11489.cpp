#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11489.h"

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
    U11489 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), case_(), s_(), win_() {}

        operator bool()
        {
            return N_-- > 0;
        }

        solution_t& operator()()
        {
            std::vector<size_t> cnt(10);
            size_t sum(0);

            for (size_t i = 0; i < s_.length(); i++) {
                cnt[s_[i] - '0']++;
                sum += s_[i] - '0';
            }

            int8_t flag(0);

            while (true) {
                size_t i;

                for (i = sum % 3; i < 10; i += 3) {
                    if (cnt[i] != 0) {
                        cnt[i]--;
                        sum -= i;
                        break;
                    }
                }

                if (i >= 10) {
                    break;
                }

                flag = 1 - flag;
            }

            win_ = flag == 1;
            case_++;
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            if (sol.N_ == std::numeric_limits<size_t>::max()) {
                in >> sol.N_;
            }

            sol.s_.clear();
            in >> sol.s_;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << "Case " << sol.case_ << ": " << (sol.win_ ? "S" : "T");
            return out;
        }

    private:
        size_t N_, case_;
        std::string s_;
        bool win_;
    };

}

void U11489::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
