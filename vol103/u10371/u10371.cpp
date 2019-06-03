#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10371.h"

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
    U10371 instance;
    instance();
}

namespace {
    enum class day_time_t
    {
        none, midnight, noon, am, pm
    };

    struct time_t
    {
        uint16_t hour_, minute_;
        day_time_t day_time_;
        std::string tz_;

        time_t(uint16_t hour = 0, uint16_t minute = 0, day_time_t day_time = day_time_t::none, const std::string& tz = "UTC")
            : hour_(hour), minute_(minute), day_time_(day_time), tz_(tz) {}

        time_t& operator=(int64_t raw);

        void normalize();
        int64_t to_raw() const;

        friend std::istream& operator>>(std::istream& in, time_t& time);

        friend std::ostream& operator<<(std::ostream& out, const time_t& time);
    };

    class solution_t {
    public:
        solution_t() : no_cases_(std::numeric_limits<size_t>::max()), tz_(  //
        {
            {"UTC", 0},          {"GMT", 0},          {"BST", 60}, //
            {"IST", 60},         {"WET", 0},          {"WEST", 60}, //
            {"CET", 60},         {"CEST", 120},       {"EET", 120}, //
            {"EEST", 180},       {"MSK", 180},        {"MSD", 240}, //
            {"AST", -240},       {"ADT", -180},       {"NST", -210}, //
            {"NDT", -150},       {"EST", -300},       {"EDT", -240}, //
            {"CST", -360},       {"CDT", -300},       {"MST", -420}, //
            {"MDT", -360},       {"PST", -480},       {"PDT", -420}, //
            {"HST", -600},       {"AKST", -540},      {"AKDT", -480}, //
            {"AEST", 600},       {"AEDT", 660},       {"ACST", 570}, //
            {"ACDT", 630},       {"AWST", 480}
        }), src_(), dst_tz_(), dst_() {}

        operator bool()
        {
            return no_cases_-- > 0;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t no_cases_;
        const std::map<std::string, int32_t> tz_;
        time_t src_;
        std::string dst_tz_;
        time_t dst_;

        time_t from_UTC(const time_t&& src);
        time_t to_UTC(const time_t& src);
    };

    solution_t& solution_t::operator()()
    {
        dst_ = from_UTC(to_UTC(src_));
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        if (sol.no_cases_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.no_cases_;
            in.ignore();
        }

        std::string line;

        if (std::getline(in, line))
        {
            std::stringstream sin(line);
            sin >> sol.src_ >> sol.dst_tz_;
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << sol.dst_;
        return out;
    }

    void time_t::normalize()
    {
        hour_ %= 24;

        if (hour_ >= 12)
        {
            day_time_ = (hour_ == 12 && minute_ == 0) ? day_time_t::noon : day_time_t::pm;
        }
        else
        {
            day_time_ = (hour_ == 0 && minute_ == 0) ? day_time_t::midnight : day_time_t::am;
        }
    }

    int64_t time_t::to_raw() const
    {
        int64_t raw = hour_ * 60 + minute_;
        return raw;
    }

    time_t& time_t::operator =(int64_t raw)
    {
        if (raw < 0)
        {
            raw += 24 * 60;
        }

        minute_ = raw % 60;
        hour_ = raw / 60;
        normalize();
        return *this;
    }

    time_t solution_t::to_UTC(const time_t& src)
    {
        time_t result(src.hour_, src.minute_, src.day_time_, "UTC");

        if (not(src.tz_ == "UTC" || src.tz_ == "GMT"))
        {
            auto raw(result.to_raw());
            raw -= tz_.find(src.tz_)->second;
            result = raw;
        }

        return result;
    }

    time_t solution_t::from_UTC(const time_t&& src)
    {
        time_t result(src.hour_, src.minute_, src.day_time_, dst_tz_);

        if (src.tz_ != dst_tz_ && not(dst_tz_ == "UTC" || dst_tz_ == "GMT"))
        {
            auto raw(result.to_raw());
            raw += tz_.find(dst_tz_)->second;
            result = raw;
        }

        return result;
    }

    std::istream& operator>>(std::istream& in, time_t& time)
    {
        std::string part;
        in >> part;

        if (part == "midnight")
        {
            time.hour_ = 0, time.minute_ = 0, time.day_time_ = day_time_t::midnight;
        }
        else if (part == "noon")
        {
            time.hour_ = 12, time.minute_ = 0, time.day_time_ = day_time_t::noon;
        }
        else
        {
            std::stringstream tmp(part);
            char del;
            tmp >> time.hour_ >> del >> time.minute_;
            in >> part;
            time.day_time_ = part == "a.m." ? day_time_t::am : day_time_t::pm;

            if (time.day_time_ == day_time_t::pm && time.hour_ != 12)
            {
                time.hour_ += 12;
            }
            else if (time.day_time_ == day_time_t::am && time.hour_ == 12)
            {
                time.hour_ = 0;
            }
        }

        in >> time.tz_;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const time_t& time)
    {
        switch (time.day_time_)
        {
        case day_time_t::midnight:
            out << "midnight";
            break;

        case day_time_t::noon:
            out << "noon";
            break;

        default:
            if (time.day_time_ == day_time_t::pm)
            {
                out << (time.hour_ == 12 ? time.hour_ : time.hour_ - 12) << ':' << std::setw(2) << std::setfill('0') << time.minute_
                    << " p.m.";
            }
            else
            {
                out << (time.hour_ == 0 ? 12 : time.hour_) << ':' << std::setw(2) << std::setfill('0') << time.minute_
                    << " a.m.";
            }

            break;
        }

        return out;
    }

}

void U10371::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
