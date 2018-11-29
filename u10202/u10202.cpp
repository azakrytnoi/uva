#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10202.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10202 instance;
    instance();
}

namespace {
    class solution {
    private:
        std::vector<int32_t> result_;
        std::vector<int32_t> summ_;

    public:
        solution() : result_(), summ_() {}

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return !summ_.empty();
        }

        solution& operator ()();

    private:
        bool calculate(size_t start, std::vector<bool> taken);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.result_.clear();
        sol.summ_.clear();
        int32_t n;

        if (in >> n) {
            int32_t limit = (n * (n - 1)) / 2;
            sol.result_.reserve(limit);
            sol.summ_.reserve(limit);
            std::generate_n(std::back_inserter(sol.summ_), limit, [&]() {
                int32_t tmp;
                in >> tmp;
                return tmp;
            });
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.result_.empty()) {
            out << "Impossible";

        } else {
            std::ostream_iterator<int32_t> io(out, " ");
            std::copy(sol.result_.begin(), sol.result_.end(), io);
            out << '\b';
        }

        return out;
    }

    solution& solution::operator()()
    {
        std::sort(summ_.begin(), summ_.end());

        std::vector<bool> taken(summ_.size());
        calculate(2, taken);

        std::sort(result_.begin(), result_.end());
        return *this;
    }

    bool solution::calculate(size_t start, std::vector<bool> taken)
    {
        if (start == summ_.size()) {
            return true;
        }

        size_t pivot = ((result_.size() - 1) * result_.size()) / 2;

        for (size_t i = 2; i < summ_.size(); i++) {
            if (start == 2) {
                double candidate = (summ_[0] + summ_[1] - summ_[i]) / 2.0;

                if (std::fabs(candidate - int32_t(candidate)) > 0.00000001) {
                    continue;
                }

                result_.push_back(int32_t(candidate));
                result_.push_back(summ_[0] - int32_t(candidate));
                result_.push_back(summ_[1] - int32_t(candidate));
                taken[i] = true;

            } else if (start == pivot) {
                if (taken[i]) {
                    continue;
                }

                result_.push_back(summ_[i] - result_[0]);
                taken[i] = true;

            } else {
                pivot = ((result_.size() - 2) * (result_.size() - 1)) / 2;

                if (taken[i]) {
                    continue;
                }

                if (summ_[i] - result_.back() != result_[start % pivot]) {
                    continue;
                }

                taken[i] = true;
            }

            if (calculate(start + 1, taken)) {
                return true;
            }

            taken[i] = false;

            if (start == 2) {
                result_.clear();
            }

            if (start == pivot) {
                result_.pop_back();
            }
        }

        return false;
    }
}

void U10202::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
