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

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10196 instance;
    instance();
}

namespace {

    class desk {
    public:
        std::vector<std::string> field_;
        std::pair<int, int> wKing_;
        std::pair<int, int> bKing_;

        desk() :
            field_(), wKing_(0, 0), bKing_(0, 0)
        {
            field_.reserve(8);
        }

        friend std::istream& operator >>(std::istream& in, desk& d)
        {
            std::string line;
            d.field_.clear();
            d.wKing_ = {0, 0 };
            d.bKing_ = {0, 0 };
            std::generate_n(std::back_inserter(d.field_), 8, [&]()
            {
                std::getline(in, line);
                size_t kPos;

                if ((kPos = line.find('K')) != std::string::npos)
                {
                    d.wKing_ = {d.field_.size(), kPos };
                }

                if ((kPos = line.find('k')) != std::string::npos)
                {
                    d.bKing_ = {d.field_.size(), kPos };
                }

                return line;
            });
            std::getline(in, line);
            return in;
        }

        operator bool() const
        {
            return wKing_.first + wKing_.second + bKing_.first + bKing_.second != 0;
        }
    };

    class solution {
    public:
        solution() :
            desk_(), wKingCheck_(false), bKingCheck_(false)
        {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return bool(desk_);
        }

        solution& operator()();

    private:
        desk desk_;
        bool wKingCheck_;
        bool bKingCheck_;

        bool traceDanger(const std::pair<int, int>& kPosition, int direction) const;
    };

    solution& solution::operator ()()
    {
        wKingCheck_ = traceDanger(desk_.wKing_, -1);
        bKingCheck_ = traceDanger(desk_.bKing_, 1);
        return *this;
    }

    std::istream& operator >>(std::istream& in, solution& sol)
    {
        sol.wKingCheck_ = sol.bKingCheck_ = false;
        in >> sol.desk_;
        return in;
    }

    std::ostream& operator <<(std::ostream& out, const solution& sol)
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

    bool solution::traceDanger(const std::pair<int, int>& kPosition, const int direction) const
    {
        bool check(false);
        std::pair<int, int> checkPosition;
        bool safe;
        auto checkDiagonal = [&] ()
        {
            char figure;

            switch (figure = desk_.field_[checkPosition.first][checkPosition.second])
            {
            case 'p':
            case 'P':
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
        auto checkDirect = [&]()
        {
            char figure;

            switch (figure = desk_.field_[checkPosition.first][checkPosition.second])
            {
            case 'q':
            case 'Q':
            case 'r':
            case 'R':
                if (direction < 0 && (figure == 'Q' || figure == 'R'))
                {
                    safe = true;

                }
                else if (direction > 0 && (figure == 'q' || figure == 'r'))
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
        auto checkKnight = [&]()
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

            case 'q':
            case 'Q':
            case 'r':
            case 'R':
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

        for (safe = false, checkPosition = {kPosition.first - 1, kPosition.second - 1 };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.first--, checkPosition.second--)
        {
            checkDiagonal();
        }

        for (safe = false, checkPosition = {kPosition.first + 1, kPosition.second - 1 };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.first++, checkPosition.second--)
        {
            checkDiagonal();
        }

        for (safe = false, checkPosition = {kPosition.first - 1, kPosition.second + 1 };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.first--, checkPosition.second++)
        {
            checkDiagonal();
        }

        for (safe = false, checkPosition = {kPosition.first + 1, kPosition.second + 1 };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.first++, checkPosition.second++)
        {
            checkDiagonal();
        }

        for (safe = false, checkPosition = {kPosition.first - 1, kPosition.second };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.first--)
        {
            checkDirect();
        }

        for (safe = false, checkPosition = {kPosition.first + 1, kPosition.second };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.first++)
        {
            checkDirect();
        }

        for (safe = false, checkPosition = {kPosition.first, kPosition.second - 1 };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.second--)
        {
            checkDirect();
        }

        for (safe = false, checkPosition = {kPosition.first, kPosition.second + 1 };
                !check && !safe && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7;
                checkPosition.second++)
        {
            checkDirect();
        }

        checkPosition = {kPosition.first - 2, kPosition.second - 1 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first - 1, kPosition.second - 2 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first + 1, kPosition.second - 2 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first + 2, kPosition.second - 1 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first + 2, kPosition.second + 1 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first + 1, kPosition.second + 2 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first - 1, kPosition.second + 2 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        checkPosition = {kPosition.first - 2, kPosition.second + 1 };

        if (!check && checkPosition.first >= 0 && checkPosition.second >= 0 && checkPosition.first <= 7 && checkPosition.second <= 7)
        {
            checkKnight();
        }

        return check;
    }

}

void U10196::operator()() const
{
    solution sol;
    int gameNo(0);

    while (std::cin >> sol && sol)
    {
        std::cout << "Game #" << (++gameNo) << ": " << sol() << std::endl;
    }
}
