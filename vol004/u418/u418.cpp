#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u418.h"

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
    U418 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : molecules_(), area_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return not molecules_.empty();
        }
        solution_t& operator()();

    private:
        std::vector<std::string> molecules_;
        size_t area_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.molecules_.clear();
        std::string line;

        if (in >> line && line != "Q")
        {
            sol.molecules_.resize(4);
            sol.molecules_[0] = line;
            in >> sol.molecules_[1] >> sol.molecules_[2] >> sol.molecules_[3];
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << sol.area_;
        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::vector<size_t> order({0, 1, 2, 3});
        area_ = std::numeric_limits<size_t>::min();
        // range [first_possible, last_possible] ==> [1, molecule.length(=12) - 2]
        const size_t last_possible = molecules_[0].size() - 2;
        const size_t first_possible = 1;
        const size_t min_gap = 2;

        do
        {
            auto& vertical_left(molecules_[order[0]]), //
                  horizontal_up(molecules_[order[1]]), //
                  vertical_right(molecules_[order[2]]), //
                  horizontal_down(molecules_[order[3]]);

            for (size_t vl_hu = first_possible; vl_hu <= last_possible; vl_hu++)
            {
                for (size_t hu_vl = first_possible; hu_vl <= last_possible; hu_vl++)
                {
                    if (vertical_left[vl_hu] != horizontal_up[hu_vl])
                    {
                        continue;
                    }

                    for (size_t hu_vr = hu_vl + min_gap; hu_vr <= last_possible; hu_vr++)
                    {
                        for (size_t vr_hu = first_possible; vr_hu <= last_possible; vr_hu++)
                        {
                            if (horizontal_up[hu_vr] != vertical_right[vr_hu])
                            {
                                continue;
                            }

                            for (size_t vl_hd = vl_hu + min_gap; vl_hd <= last_possible; vl_hd++)
                            {
                                for (size_t hd_vl = first_possible; hd_vl <= last_possible; hd_vl++)
                                {
                                    if (vertical_left[vl_hd] != horizontal_down[hd_vl])
                                    {
                                        continue;
                                    }

                                    size_t vr_hd = vr_hu + vl_hd - vl_hu;
                                    size_t hd_vr = hd_vl + hu_vr - hu_vl;

                                    if (vr_hd >= first_possible && vr_hd <= last_possible &&
                                            hd_vr >= first_possible && hd_vr <= last_possible &&
                                            vertical_right[vr_hd] == horizontal_down[hd_vr])
                                    {
                                        area_ = std::max(area_, (vl_hd - vl_hu - 1) * (hu_vr - hu_vl - 1));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        while (std::next_permutation(order.begin(), order.end()));

        return *this;
    }

}

void U418::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
