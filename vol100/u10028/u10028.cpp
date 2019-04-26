#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10028.h"

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
    U10028 instance;
    instance();
}

namespace {

    struct date_t
    {
        uint16_t year_, month_, day_;

        date_t(uint16_t year = 0, uint16_t month = 0, uint16_t day = 0) : year_(year), month_(month), day_(day) {}

        date_t anniversary()
        {
            date_t temp(*this);
            temp.year_++;
            return temp;
        }

        bool operator < (const date_t& rhs)
        {
            if (year_ == rhs.year_)
            {
                if (month_ == rhs.month_)
                {
                    return day_ < rhs.day_;
                }

                return month_ < rhs.month_;
            }

            return year_ < rhs.year_;
        }

        bool operator == (const date_t& rhs)
        {
            return (year_ == rhs.year_) && (month_ == rhs.month_) && (day_ == rhs.day_);
        }

        friend std::istream& operator>>(std::istream& in, date_t& rhs)
        {
            uint32_t temp(0);
            in >> temp;
            rhs.day_ = temp % 100;
            temp /= 100;
            rhs.month_ = temp % 100;
            rhs.year_ = temp / 100;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const date_t& rhs)
        {
            out << std::fixed << std::setw(4) << rhs.year_ << '-' << std::setw(2) << std::setfill('0') << rhs.month_
                << '-' << std::setw(2) << rhs.day_;
            return out;
        }
    };

    struct event_t
    {
        date_t date_;
        int32_t points_;

        event_t() : date_(), points_() {}

        friend std::istream& operator>>(std::istream& in, event_t& rhs)
        {
            in >> rhs.date_ >> rhs.points_;
            return in;
        }
    };

    struct status_t
    {
        date_t date_;
        int32_t merit_, demerit_;

        status_t(date_t& date) : date_(date), merit_(), demerit_() {}

        friend std::ostream& operator<<(std::ostream& out, const status_t& rhs)
        {
            out << rhs.date_ << ' ';

            if (rhs.merit_ == 0 && rhs.demerit_ == 0)
            {
                out << "No merit or demerit points.";
            }
            else
            {
                if (rhs.merit_ > 0)
                {
                    out << rhs.merit_ << " merit point(s).";
                }
                else
                {
                    out << rhs.demerit_ << " demerit point(s).";
                }
            }

            return out;
        }
    };

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), events_(), statuses_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::vector<event_t> events_;
        std::vector<status_t> statuses_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
            in.ignore(2);
        }

        sol.events_.clear();
        sol.statuses_.clear();
        date_t start;

        if (in >> start)
        {
            in.ignore();
            sol.statuses_.push_back(start);
            std::string line;

            while (std::getline(in, line) && not line.empty())
            {
                std::stringstream ss(line);
                event_t event;
                ss >> event;
                sol.events_.push_back(event);
            }

            sol.events_.shrink_to_fit();
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<status_t> sout (out, "\n");
        std::copy(sol.statuses_.begin(), sol.statuses_.end(), sout);
        return out;
    }

    solution_t& solution_t::operator()()
    {
        status_t current (statuses_.front());
        auto offence(events_.begin());
        date_t last(offence == events_.end() ? current.date_ : offence->date_);
        bool changed(false);

        auto process_offence = [&]()
        {
            current.date_ = offence->date_;

            if (offence->points_ > current.merit_ * 2)
            {
                current.demerit_ += offence->points_ - current.merit_ * 2;
                current.merit_ = 0;
            }
            else
            {
                current.merit_ = std::max(current.merit_ - offence->points_ / 2, 0);
            }

            ++offence;
            changed = true;
        };

        auto award_demerit = [&]()
        {
            current.demerit_ -= std::max(2, (current.demerit_ + 1) / 2);
            current.demerit_ = std::max(0, current.demerit_);

            if (current.demerit_ == 0)
            {
                last = current.date_;
            }

            changed = true;
        };

        auto award_merit = [&]()
        {
            current.merit_++;
            last = current.date_;
            changed = true;
        };

        while (current.merit_ < 5)
        {
            current.date_ = current.date_.anniversary();
            changed = false;

            if (offence != events_.end() && offence->date_ < current.date_)
            {
                process_offence();
            }
            else
            {
                if (current.demerit_ > 0)
                {
                    award_demerit();
                }
                else if (current.date_.year_ - last.year_ >= 2)
                {
                    award_merit();
                }

                if (offence != events_.end() && current.date_ == offence->date_)
                {
                    process_offence();
                }
            }

            if (changed)
            {
                statuses_.push_back(current);
            }
        }

        return *this;
    }

}

void U10028::operator()() const
{
    solution_t sol;

    while (true)
    {
        std::cin >> sol;

        if (sol)
        {
            std::cout << sol() << std::endl;
        }
        else
        {
            break;
        }
    }
}
