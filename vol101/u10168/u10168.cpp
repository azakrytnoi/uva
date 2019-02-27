#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10168.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

#include "prime.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10168 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution();
        ~solution()
        {
            delete sieve_;
        }

        solution(const solution& rhs) = delete;
        solution& operator = (const solution& rhs) = delete;

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        solution& operator()();

    private:
        math::sieve_t<10000001>* sieve_;
        int32_t n_;
        std::vector<int32_t> summ_;
    };

    solution::solution() :
        sieve_(nullptr), n_(), summ_()
    {
        sieve_ = new math::sieve_t<10000001>;
    }

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        in >> sol.n_;
        sol.summ_.clear();
        return in;
    }

    std::ostream& operator <<(std::ostream& out, const solution& sol)
    {
        if (sol.summ_.size() < 4) {
            out << "Impossible.";

        } else {
            out << sol.summ_[0] << ' ' << sol.summ_[1] << ' ' << sol.summ_[2] << ' ' << sol.summ_[3];

            //        std::ostream_iterator<int32_t> oit(out, " ");
            //        std::copy(sol.summ_.begin(), sol.summ_.end(), oit);
        }

        return out;
    }

    solution& solution::operator()()
    {
        if (n_ >= 8) {
            summ_.resize(2);

            if (n_ % 2 == 0) {
                summ_[0] = summ_[1] = 2;
                n_ -= 4;

            } else {
                summ_[0] = 2;
                summ_[1] = 3;
                n_ -= 5;
            }

            for (int32_t i = 2, e = n_ / 2; i <= e; ++i) {
                if (sieve_->is_prime(i) && sieve_->is_prime(n_ - i)) {
                    summ_.resize(4);
                    summ_[2] = i;
                    summ_[3] = n_ - i;
                    break;
                }
            }
        }

        return *this;
    }
}

void U10168::operator()() const
{
    solution sol;

    while (std::cin >> sol) {
        std::cout << sol() << std::endl;
    }
}
