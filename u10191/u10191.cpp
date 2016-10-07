#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10191.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10191 instance;
    instance();
}

namespace
{
struct time_point {
    int16_t hour_;
    int16_t minute_;
};

bool operator < (const time_point& tpl, const time_point& tpr)
{
    if (tpl.hour_ == tpr.hour_) {
        return tpl.minute_ < tpr.minute_;
    }

    return tpl.hour_ < tpr.hour_;
}

struct appointment {
    time_point start_;
    time_point end_;
};

std::istream& operator >> (std::istream& in, time_point& tp)
{
    char buff;
    in >> tp.hour_ >> buff >> tp.minute_;
    return in;
}

std::ostream& operator << (std::ostream& out, const time_point& tp)
{
    out << tp.hour_ / 10 << tp.hour_ % 10 << ':' << tp.minute_ / 10 << tp.minute_ % 10;
    return out;
}

std::istream& operator >> (std::istream& in, appointment &app)
{
    in >> app.start_ >> app.end_;
    return in;
}

struct nap_time {
    int16_t duration_;
    time_point start_;
};

int16_t operator - (const time_point& tpl, const time_point& tpr)
{
    return tpl.hour_ * 60 + tpl.minute_ - tpr.hour_ * 60 - tpr.minute_;
}

std::ostream& operator << (std::ostream& out, const nap_time& nap)
{
    std::stringstream oss;
    if (nap.duration_ > 60) {
        oss << nap.duration_ / 60 << " hours and ";
    }

    out << "starts at " << nap.start_ << " and will last for " << oss.str() << nap.duration_ % 60 << " minutes.";
    return out;
}

class solution
{
public:
    solution() :agenda_(), nap_(
    {
        0, { 10, 0 }
    }), counter_(0) {}

    friend std::istream& operator >> (std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool();

private:
    std::vector<appointment> agenda_;
    nap_time nap_;
    uint16_t counter_;
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    sol.agenda_.clear();
    sol.nap_ = { 0,{ 10, 0 } };
    int16_t n;
    in >> n;
    if (in && n > 0) {
        sol.counter_++;
        std::string line;
        std::getline(in, line);
        sol.agenda_.reserve(n);
        std::generate_n(std::back_inserter(sol.agenda_), n, [&]() {
            std::getline(in, line);
            appointment app;
            std::stringstream iss(line);
            iss >> app;
            return app;
        });
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
    out << "Day #" << sol.counter_ << ": the longest nap " << sol.nap_;
    return out;
}

solution::operator bool()
{
    if (agenda_.empty()) {
        return false;
    }

    std::sort(agenda_.begin(), agenda_.end(), [](const appointment& app1, const appointment& app2) {
        return app1.start_ < app2.start_;
    });
    const time_point first = { 10, 0 };
    const time_point last = { 18, 0 };
    if (first < agenda_[0].start_) {
        nap_.duration_ = agenda_[0].start_ - first;
        nap_.start_ = first;
    }

    for (size_t i = 1; i < agenda_.size(); i++) {
        if (agenda_[i - 1].end_ < agenda_[i].start_ && nap_.duration_ < (agenda_[i].start_ - agenda_[i - 1].end_)) {
            nap_.duration_ = agenda_[i].start_ - agenda_[i - 1].end_;
            nap_.start_ = agenda_[i - 1].end_;
        }
    }

    if (agenda_.back().end_ < last && nap_.duration_ < (last - agenda_.back().end_)) {
        nap_.duration_ = last - agenda_.back().end_;
        nap_.start_ = agenda_.back().end_;
    }

    return true;
}
}

void U10191::operator()() const
{
    solution sol;
    do {
        std::cin >> sol;
        if (sol) {
            std::cout << sol << std::endl;
        }
    } while (std::cin);
}
