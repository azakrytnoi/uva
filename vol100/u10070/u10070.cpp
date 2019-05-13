#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10070.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>

#include "biguint.h"

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10070 instance;
    instance();
}

namespace {

    enum class kind_t
    {
        leap, huluculu, bulukulu
    };

    class solution_t {
    public:
        solution_t() : year_(), kind_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        math::uint_big_t year_;
        std::set<kind_t> kind_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        in >> sol.year_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.kind_.empty())
        {
            out << "This is an ordinary year." << std::endl;
        }
        else
        {
            if (sol.kind_.find(kind_t::leap) != sol.kind_.end())
            {
                out << "This is leap year." << std::endl;
            }

            if (sol.kind_.find(kind_t::huluculu) != sol.kind_.end())
            {
                out << "This is huluculu festival year." << std::endl;
            }

            if (sol.kind_.find(kind_t::bulukulu) != sol.kind_.end())
            {
                out << "This is bulukulu festival year." << std::endl;
            }
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        kind_.clear();
        auto is_leap = [](const math::uint_big_t& year) -> bool
        {
            static const math::uint_big_t _4(4);

            if (year % _4 == 0)
            {
                static const math::uint_big_t _100(100), _400(400);

                if (year % _100 != 0 || year % _400 == 0)
                {
                    return true;
                }
            }

            return false;
        };
        auto leap (is_leap(year_));

        if (leap)
        {
            kind_.insert(kind_t::leap);

            static math::uint_big_t _55(55);

            if (year_ % _55 == 0)
            {
                kind_.insert(kind_t::bulukulu);
            }
        }

        static math::uint_big_t _15(15);

        if (year_ % _15 == 0)
        {
            kind_.insert(kind_t::huluculu);
        }

        return *this;
    }

}

void U10070::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
