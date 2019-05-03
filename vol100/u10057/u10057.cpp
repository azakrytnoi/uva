#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10057.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10057 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : n_(), X_(), mid_(), midp_(), midn_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not X_.empty();
        }
        solution_t& operator()();

    private:
        size_t n_;
        std::vector<uint16_t> X_;
        size_t mid_, midp_, midn_;
    };

    template<typename _Container>
    class count_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
    private:
        _Container* container_;

    public:
        typedef _Container          container_type;

        explicit
        count_insert_iterator(_Container& __x)
            : container_(std::__addressof(__x)) { }

        count_insert_iterator& operator=(const typename _Container::value_type& __key)
        {
            (*container_)[__key]++;
            return *this;
        }

        count_insert_iterator& operator=(typename _Container::value_type&& __key)
        {
            (*container_)[__key]++;
            return *this;
        }

        count_insert_iterator& operator*()
        {
            return *this;
        }

        count_insert_iterator& operator++()
        {
            return *this;
        }

        count_insert_iterator operator++(int)
        {
            return *this;
        }
    };

    template<typename _Container>
    inline count_insert_iterator<_Container>
    count_inserter(_Container& __x)
    {
        return count_insert_iterator<_Container>(__x);
    }

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.X_.clear();

        if (in >> sol.n_)
        {
            sol.X_.resize(std::numeric_limits<uint16_t>::max());
            std::istream_iterator<uint16_t> iin(in);
            std::copy_n(iin, sol.n_, count_inserter(sol.X_));
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.mid_ << ' ' << sol.midn_ << ' ' << sol.midp_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        auto part_summ = [&](size_t max) -> size_t
        {
            size_t summ(0);

            for (auto i = X_.begin(); ; ++i)
            {
                summ += *i;

                if (summ >= max)
                {
                    return std::distance(X_.begin(), i);
                }
            }
        };

        if (n_ & 1)
        {
            mid_ = part_summ(n_ / 2 + 1);
            midn_ = X_[mid_];
            midp_ = 1;
        }
        else
        {
            mid_ = part_summ(n_ / 2);
            midn_ = X_[mid_];
            auto next = part_summ(n_ / 2 + 1);

            if (next != mid_)
            {
                midn_ += X_[next];
                midp_ = next - mid_ + 1;
            }
            else
            {
                midp_ = 1;
            }
        }

        return *this;
    }

}

void U10057::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
