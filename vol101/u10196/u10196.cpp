#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10196.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <functional>
#include <cctype>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10196 instance;
    instance();
}

namespace {

    struct position_t : public std::pair<int, int>
    {
        position_t(int x = 0, int y = 0) : std::pair<int, int>(x, y) {}
        position_t(size_t x = 0, size_t y = 0) : std::pair<int, int>(x, y) {}

        bool within ()
        {
            return first >= 0 && second >= 0 && first <= 7 && second <= 7;
        }

        position_t& operator += (const position_t& rhs)
        {
            first += rhs.first, second += rhs.second;
            return *this;
        }

        friend position_t operator + (const position_t& lhs, const position_t& rhs)
        {
            return {lhs.first + rhs.first, lhs.second + rhs.second};
        }
    };

    typedef std::pair<position_t, std::function<void(position_t&)>> delta_t;

    std::ostream& operator <<(std::ostream& out, const position_t& p)
    {
        out << '(' << p.first << ',' << p.second << ')';
        return out;
    }

    class desk_t {
    public:
        std::vector<std::string> field_;
        position_t wKing_;
        position_t bKing_;

        desk_t() :
            field_(), wKing_{0, 0}, bKing_{0, 0}
        {
            field_.reserve(8);
        }

        friend std::istream& operator >>(std::istream& in, desk_t& d)
        {
            std::string line;
            d.field_.clear();
            d.wKing_ = {0, 0};
            d.bKing_ = {0, 0};
            std::generate_n(std::back_inserter(d.field_), 8, [&]()
            {
                std::getline(in, line);
                size_t kPos;

                if ((kPos = line.find('K')) != std::string::npos)
                {
                    d.wKing_ = {d.field_.size(), kPos};
                }

                if ((kPos = line.find('k')) != std::string::npos)
                {
                    d.bKing_ = {d.field_.size(), kPos};
                }

                return line;
            });
            std::getline(in, line);
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const desk_t& d)
        {
            out << "-- " << d.bKing_ << ' ' << d.wKing_ << std::endl;
            std::ostream_iterator<std::string> sout(out, "\n");
            std::copy(d.field_.begin(), d.field_.end(), sout);
            out << "----------------------" << std::endl;
            return out;
        }

        operator bool() const
        {
            return wKing_.first + wKing_.second + bKing_.first + bKing_.second != 0;
        }
    };

    class solution_t {
    public:
        solution_t() :
            desk_(), wKingCheck_(false), bKingCheck_(false)
        {}

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return bool(desk_);
        }

        solution_t& operator()();

    private:
        desk_t desk_;
        bool wKingCheck_;
        bool bKingCheck_;

        bool traceDanger(const position_t& kPosition, int direction) const;
        bool traceDiagonal(const position_t& kPosition, int direction) const;
        bool traceDirect(const position_t& kPosition, int direction) const;
        bool traceKnight(const position_t& kPosition, int direction) const;
    };

    solution_t& solution_t::operator ()()
    {
        wKingCheck_ = traceDanger(desk_.wKing_, -1);
        bKingCheck_ = traceDanger(desk_.bKing_, 1);
        return *this;
    }

    std::istream& operator >>(std::istream& in, solution_t& sol)
    {
        sol.wKingCheck_ = sol.bKingCheck_ = false;
        in >> sol.desk_;
        return in;
    }

    std::ostream& operator <<(std::ostream& out, const solution_t& sol)
    {
        if (sol.wKingCheck_)
        {
            out << "white king is in check.";
        }
        else if (sol.bKingCheck_)
        {
            out << "black king is in check.";
        }
        else
        {
            out << "no king is in check.";
        }

        return out;
    }

    bool solution_t::traceDanger(const position_t& kPosition, const int direction) const
    {
        return traceDiagonal(kPosition, direction) || traceDirect(kPosition, direction) || traceKnight(kPosition, direction);
    }

    bool solution_t::traceDiagonal(const position_t& kPosition, int direction) const
    {
        position_t checkPosition(0, 0);
        bool safe, check(false);
        auto testPosition = [&] ()
        {
            char figure (desk_.field_[checkPosition.first][checkPosition.second]);
            // std::clog << desk_.field_[kPosition.first][kPosition.second] << kPosition << " -> " << figure << checkPosition << std::endl;

            switch (figure)
            {
            case 'p':
            case 'P':
            {
                auto p_distance = [&]()
                {
                    return std::abs(checkPosition.first - kPosition.first) == 1;
                };

                if (p_distance())
                {
                    if (direction < 0 && figure == 'p' && kPosition.first > checkPosition.first)
                    {
                        check = true;
                    }
                    else if (direction > 0 && figure == 'P' && kPosition.first < checkPosition.first)
                    {
                        check = true;
                    }
                    else
                    {
                        safe = true;
                    }
                }
                else
                {
                    safe = true;
                }
            }
            break;

            case 'b':
            case 'B':
            case 'q':
            case 'Q':
                if (direction < 0 && (figure == 'B' || figure == 'Q' || figure == 'P'))
                {
                    safe = true;
                }
                else if (direction > 0 && (figure == 'b' || figure == 'q' || figure == 'p'))
                {
                    safe = true;
                }
                else
                {
                    check = true;
                }

                break;

            case 'n':
            case 'N':
            case 'r':
            case 'R':
            case 'k':
            case 'K':
                safe = true;
                break;

            case '.':
            default:
                break;
            }
        };

        static const std::vector <position_t> deltas (
        {
            {-1, -1}, {1, -1}, {-1, 1}, {1, 1}
        });

        for (auto& delta : deltas)
        {
            for (safe = false, checkPosition = kPosition + delta;
                    !check && !safe && checkPosition.within();
                    checkPosition += delta)
            {
                testPosition();
            }
        }

        return check;
    }

    bool solution_t::traceDirect(const position_t& kPosition, int direction) const
    {
        position_t checkPosition(0, 0);
        bool safe, check(false);
        auto testPosition = [&]()
        {
            char figure(desk_.field_[checkPosition.first][checkPosition.second]);

            switch (figure)
            {
            case 'q':
            case 'Q':
            case 'r':
            case 'R':
                if (direction < 0 && std::isupper(figure))
                {
                    safe = true;
                }
                else if (direction > 0 && std::islower(figure))
                {
                    safe = true;
                }
                else
                {
                    check = true;
                }

                break;

            case 'n':
            case 'N':
            case 'p':
            case 'P':
            case 'b':
            case 'B':
            case 'k':
            case 'K':
                safe = true;
                break;

            case '.':
            default:
                break;
            }
        };

        static const std::vector<position_t> deltas (
        {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        });

        for (auto& delta : deltas)
        {
            for (safe = false, checkPosition = kPosition + delta;
                    !check && !safe && checkPosition.within();
                    checkPosition += delta)
            {
                testPosition();
            }
        }

        return check;
    }

    bool solution_t::traceKnight(const position_t& kPosition, int direction) const
    {
        position_t checkPosition(0, 0);
        bool check(false);
        auto testPosition = [&]()
        {
            char figure;

            switch (figure = desk_.field_[checkPosition.first][checkPosition.second])
            {
            case 'n':
            case 'N':
                if (direction < 0 && figure == 'n')
                {
                    check = true;
                }
                else if (direction > 0 && figure == 'N')
                {
                    check = true;
                }

                break;

            default:
                break;
            }
        };

        static const std::vector<position_t> deltas (
        {
            {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {1, 2}, {2, 1}, {-1, 2}, {-2, 1}
        });

        for (auto& delta : deltas)
        {
            checkPosition = kPosition + delta;

            if (!check && checkPosition.within())
            {
                testPosition();
            }
        }

        return check;
    }
}

void U10196::operator()() const
{
    solution_t sol;
    int gameNo(0);

    while (std::cin >> sol && sol)
    {
        std::cout << "Game #" << (++gameNo) << ": " << sol() << std::endl;
    }
}
