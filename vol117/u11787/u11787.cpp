#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11787.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <list>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U11787 instance;
    instance();
}

namespace {

    class solution {
        enum class unit_t : char
        {
            B = 'B', U = 'U', S = 'S', P = 'P', F = 'F', T = 'T', M = 'M', Z = '0'
        };
        enum class direction_t
        {
            undefined, left, right
        };
        struct glyph_t
        {
//        	glyph_t() : unit_(unit_t::Z), legal_(), value_() {}
            unit_t unit_;
            std::map<direction_t, std::list<unit_t>> legal_;
            uint64_t value_;
        };

        friend bool operator < (unit_t lhs, unit_t rhs)
        {
            static std::vector<std::pair<unit_t, uint64_t>> num_val (
            {
                {unit_t::B, 1},
                {unit_t::U, 10},
                {unit_t::S, 100},
                {unit_t::P, 1000},
                {unit_t::F, 10000},
                {unit_t::T, 100000},
                {unit_t::M, 1000000},
                {unit_t::Z, 0}
            });
            uint64_t lhs_val = std::find_if(num_val.begin(), num_val.end(), [&](auto & p)
            {
                return p.first == lhs;
            })->second;
            uint64_t rhs_val = std::find_if(num_val.begin(), num_val.end(),
                                            [&](auto & p)
            {
                return p.first == rhs;
            })->second;
            return lhs_val < rhs_val;
        }

    public:
        solution() : number_(), egypt_(), glyphs_(), valid_()
        {
            std::vector<unit_t> units ({unit_t::B, unit_t::U, unit_t::S, unit_t::P, unit_t::F, unit_t::T, unit_t::M});
            std::vector<std::pair<unit_t, glyph_t>> glyps;
            glyps.reserve(units.size());
            std::transform(units.begin(), units.end(), std::back_inserter(glyps), [&](unit_t unit) -> std::pair<unit_t, glyph_t>
            {
                glyph_t glyph ({unit, {}, 0});
                std::copy_if(units.begin(), units.end(), std::back_inserter(glyph.legal_[direction_t::left]), [&](unit_t uu)
                {
                    return unit < uu;
                });
                std::copy_if(units.begin(), units.end(), std::back_inserter(glyph.legal_[direction_t::right]), [&](unit_t uu)
                {
                    return uu < unit;
                });

                switch (unit)
                {
                case unit_t::B:
                    glyph.value_ = 1;
                    break;

                case unit_t::U:
                    glyph.value_ = 10;
                    break;

                case unit_t::S:
                    glyph.value_ = 100;
                    break;

                case unit_t::P:
                    glyph.value_ = 1000;
                    break;

                case unit_t::F:
                    glyph.value_ = 10000;
                    break;

                case unit_t::T:
                    glyph.value_ = 100000;
                    break;

                case unit_t::M:
                    glyph.value_ = 1000000;
                    break;

                default:
                    break;
                }
                return std::make_pair(unit, glyph);
            });
            glyphs_.insert(glyps.begin(), glyps.end());
        }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        uint64_t number_;
        std::string egypt_;
        std::map<unit_t, glyph_t> glyphs_;
        bool valid_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.number_ = 0;
        sol.valid_ = true;
        std::getline(in, sol.egypt_);
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.valid_)
        {
            out << sol.number_;
        }
        else
        {
            out << "error";
        }

        return out;
    }

    solution& solution::operator()()
    {
        direction_t dir (direction_t::undefined);
        std::stringstream in (egypt_);
        char ch;
        unit_t last(unit_t::Z);
        uint16_t same_cnt(0);

        while (valid_ && in >> ch)
        {
            unit_t unit = static_cast<unit_t>(ch);
            glyph_t& glyph = glyphs_.find(unit)->second;
            number_ += glyph.value_;

            if (last != unit_t::Z && last != unit)
            {
                same_cnt = 0;
                auto& legal = glyphs_.find(last)->second.legal_.find(direction_t::left)->second;

                if (dir == direction_t::undefined)
                {
                    dir = std::find(legal.begin(), legal.end(), unit) == legal.end() ? direction_t::right : direction_t::left;
                }
                else
                {
                    auto new_dir = std::find(legal.begin(), legal.end(), unit) == legal.end() ? direction_t::right : direction_t::left;
                    valid_ = dir == new_dir;
                }
            }
            else if (last == unit)
            {
                same_cnt++;
            }

            last = unit;
            valid_ &= same_cnt < 9;
        }

        return *this;
    }

}

void U11787::operator()() const
{
    size_t N;
    std::cin >> N;
    std::string dummy;
    std::getline(std::cin, dummy);
    solution sol;

    while (N--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
