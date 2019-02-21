#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u419.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
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
    U419 instance;
    instance();
}

namespace {

    enum class wday_t : char {
        M = 'M', T = 'T', W = 'W', R = 'R', F = 'F'
    };

    struct slot_t {
        wday_t wday_;
        uint16_t mday_, month_;
        uint16_t hour_, minute_;
        uint16_t year_;

        slot_t () : wday_(wday_t::M), mday_(1), month_(1), hour_(0), minute_(0), year_(0) {}

        bool operator == (const slot_t& rhs) const
        {
            if (year_ == rhs.year_) {
                if (month_ == rhs.month_) {
                    if (mday_ == rhs.mday_) {
                        if (hour_ == rhs.hour_) {
                            return minute_ == rhs.minute_;
                        }
                    }
                }
            }

            return false;
        }

        bool operator < (const slot_t& rhs) const
        {
            if (year_ != rhs.year_) {
                return year_ < rhs.year_;
            }

            if (month_ != rhs.month_) {
                return month_ < rhs.month_;
            }

            if (mday_ != rhs.mday_) {
                return mday_ < rhs.mday_;
            }

            if (hour_ != rhs.hour_) {
                return hour_ < rhs.hour_;
            }

            return minute_ < rhs.minute_;
        }

        slot_t& operator ++()
        {
            minute_ += 15;

            if (minute_ == 60) {
                minute_ = 0;
                ++hour_;

                if (hour_ == 17) {
                    hour_ = 9;
                    static std::map<wday_t, std::pair<wday_t, uint16_t>> next_day ({
                        {wday_t::M, {wday_t::T, 1}}, //
                        {wday_t::T, {wday_t::W, 1}}, //
                        {wday_t::W, {wday_t::R, 1}}, //
                        {wday_t::R, {wday_t::F, 1}}, //
                        {wday_t::F, {wday_t::M, 3}}
                    });
                    auto& next = next_day[wday_];
                    wday_ = next.first;
                    mday_ += next.second;
                    static std::map<uint16_t, uint16_t> max_day ({
                        {1, 31}, {2, 28}, {3, 31}, {4, 30}, {5, 31}, {6, 30}, {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}
                    });
                    auto next_month = max_day[month_];

                    if (mday_ > next_month) {
                        mday_ %= next_month;
                        ++month_;

                        if (month_ > 12) {
                            month_ = 1;
                            ++year_;
                        }
                    }
                }
            }

            return *this;
        }

        friend slot_t operator+(const slot_t& slot, uint16_t n)
        {
            slot_t temp(slot);

            while (n--) {
                ++temp;
            }

            return temp;
        }

        slot_t& operator -= (uint16_t n)
        {
            uint16_t time = hour_ * 60 + minute_;
            time -= n;
            hour_ = time / 60;
            minute_ = time % 60;
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, slot_t& slot)
        {
            char ch;
            in >> ch;
            slot.wday_ = static_cast<wday_t>(ch);
            in >> slot.month_ >> slot.mday_;
            slot.minute_ = 0;
            slot.hour_ = 9;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const slot_t& slot)
        {
            out << static_cast<char>(slot.wday_) << ' ' << slot.month_ << ' ' << slot.mday_;
            uint16_t time = slot.hour_ * 100 + slot.minute_;
            out << ' ' << std::setfill('0') << std::setw(4) << std::right << time << std::setfill(' ');
            return out;
        }

        static uint16_t duration (const slot_t& start, const slot_t& end)
        {
            if (end < start) {
                return 0;
            }

            int16_t diff = (end.hour_ * 60 + end.minute_) - (start.hour_ * 60 + start.minute_);
            return diff;
        }
    };

    class solution_t {
    public:
        solution_t() : n_(), duration_(), agenda_(), meetings_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        uint16_t n_, duration_;
        typedef std::pair<slot_t, bool> agenda_t;
        std::vector<agenda_t> agenda_;
        std::vector<slot_t> meetings_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        std::string line;
        slot_t today;
        in >> today >> sol.n_ >> sol.duration_;
        sol.agenda_.reserve(4 * (17 - 9) * 52);
        sol.agenda_.push_back(std::make_pair(today, true));

        for (size_t cnt = 0; cnt < 4 * (17 - 9) * 52; ++cnt) {
            slot_t next = sol.agenda_.back().first;
            sol.agenda_.push_back(std::make_pair(++next, true));
        }

        sol.agenda_.shrink_to_fit();
        in.ignore();

        while (std::getline(in, line) && line != "done") {
            while (std::getline(in, line) && line != "done") {
                std::stringstream sin(line);
                slot_t mstart;
                uint16_t start(0), end(0);
                sin >> mstart >> start >> end;
                slot_t mend(mstart);
                mend.hour_ = end / 100;
                mend.minute_ = end % 100;
                mstart.hour_ = start / 100;
                mstart.minute_ = start % 100;
                std::pair<slot_t, bool> agenda_start = std::make_pair(mstart, false);
                auto meet_start = std::lower_bound(sol.agenda_.begin(), sol.agenda_.end(), agenda_start,
                [](const solution_t::agenda_t& lhs, const solution_t::agenda_t& rhs) -> bool {
                    return lhs.first < rhs.first;
                });

                while (meet_start->first < mend) {
                    meet_start->second = false;
                    ++meet_start;
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<slot_t> sout(out, "\n");
        std::copy(sol.meetings_.begin(), sol.meetings_.end(), sout);

        if (sol.meetings_.size() < sol.n_) {
            out << "No more times available" << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        meetings_.reserve(n_);
        auto last = agenda_.back();
        last.first -= duration_;
        auto last_i = std::lower_bound(agenda_.begin(), agenda_.end(), last,
        [](const agenda_t& lhs, const agenda_t& rhs) -> bool {
            return lhs.first < rhs.first;
        });

        for (auto meet = agenda_.begin(); meetings_.size() < n_ && meet != last_i; ++meet) {
            if (meet->second) {
                auto possible = meet->first;
                auto end_of_day(possible);
                end_of_day.hour_ = 17;
                end_of_day.minute_ = 0;

                if (slot_t::duration(possible, end_of_day) >= duration_) {

                    while (slot_t::duration (possible, meet->first) < duration_ && meet->second && meet != last_i) {
                        ++meet;
                    }

                    if (slot_t::duration(possible, meet->first) == duration_) {
                        meetings_.push_back(possible);
                    }

                    if (meet == agenda_.end()) {
                        break;
                    }
                } else {
                    while (meet->first < end_of_day && meet != last_i) {
                        ++meet;
                    }

                    --meet;
                }
            }
        }

        return *this;
    }

}

void U419::operator()() const
{
    solution_t sol;

    std::cin >> sol;
    std::cout << sol();
}
