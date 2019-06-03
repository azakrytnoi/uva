#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u212.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U212 instance;
    instance();
}

namespace {

    class time {
        uint16_t hour_;
        uint16_t minute_;
    public:
        time(uint16_t hour = 0, uint16_t minute = 0) : hour_(hour), minute_(minute) {}
        time(const time& rhs) : hour_(rhs.hour_), minute_(rhs.minute_) {}
        time& operator = (uint16_t hour)
        {
            hour_ = hour;
            minute_ = 0;
            return *this;
        }

        time& operator += (uint32_t minutes)
        {
            minute_ += minutes;
            hour_ += (minute_ / 60);
            minute_ = minute_ % 60;
            return *this;
        }

        friend bool operator < (const time& lhs, const time& rhs)
        {
            return (lhs.hour_ * 60 + lhs.minute_) < (rhs.hour_ * 60 + rhs.minute_);
        }

        friend bool operator == (const time& lhs, const time& rhs)
        {
            return (lhs.hour_ * 60 + lhs.minute_) == (rhs.hour_ * 60 + rhs.minute_);
        }

        friend time operator+(const time& original, uint32_t minutes)
        {
            time new_time (original);
            new_time += minutes;
            return new_time;
        }

        friend uint32_t operator - (const time& lhs, const time& rhs)
        {
            return (lhs.hour_ * 60 + lhs.minute_) - (rhs.hour_ * 60 + rhs.minute_);
        }

        friend std::ostream& operator << (std::ostream& out, const time& rhs)
        {
            out << std::setw(2) << std::right << rhs.hour_ << std::setw(1) << ':' << std::setw(2) << std::setfill('0') << rhs.minute_ << std::setfill(' ');
            return out;
        }
    };

    class resource {
    protected:
        uint16_t id_;
        uint32_t used_;
        resource(uint16_t id) : id_(id), used_() {}
    public:
        double utilized(uint32_t total)
        {
            return double(used_ * 100.0) / total;
        }

        void use (uint32_t duration)
        {
            used_ += duration;
        }

        uint16_t id() const
        {
            return id_;
        }
    };

    class room : public resource {
    public:
        room (uint16_t id) : resource(id) {}

        friend std::ostream& operator << (std::ostream& out, const room& rhs)
        {
            out << "Room" << std::right << std::setw(4) << rhs.id_ << std::setw(6) << rhs.used_;
            return out;
        }

        struct less : public std::binary_function<std::shared_ptr<room>, std::shared_ptr<room>, bool>
        {
            bool operator() (const std::shared_ptr<room>& lhs, const std::shared_ptr<room>& rhs)
            {
                return lhs->id_ > rhs->id_;
            }
        };
    };

    class bed : public resource {
    public:
        bed (uint16_t id) : resource(id) {}

        friend std::ostream& operator << (std::ostream& out, const bed& rhs)
        {
            out << "Bed" << std::right << std::setw(5) << rhs.id_ << std::setw(6) << rhs.used_;
            return out;
        }
        struct less : public std::binary_function<std::shared_ptr<bed>, std::shared_ptr<bed>, bool>
        {
            bool operator() (const std::shared_ptr<bed>& lhs, const std::shared_ptr<bed>& rhs)
            {
                return lhs->id_ > rhs->id_;
            }
        };
    };

    template<typename R>
    class resource_ref {
        std::shared_ptr<R> resource_;
        const time assigned_;
        const time released_;
    public:
        resource_ref(std::shared_ptr<R> resource, const time& assigned, uint16_t duration) : resource_(resource), assigned_(assigned),
            released_(assigned + duration)
        {
            resource_->use(duration);
        }

        const time& assigned() const
        {
            return assigned_;
        }
        const time& released() const
        {
            return released_;
        }

        std::shared_ptr<R> resource() const
        {
            return resource_;
        }

        friend std::ostream& operator << (std::ostream& out, const resource_ref& rhs)
        {
            out << std::setw(4) << rhs.resource_->id() << std::setw(3) << ' ' << rhs.assigned() << std::setw(2) << ' ' << rhs.released();
            return out;
        }
    };

    class patient {
        uint16_t id_;
        std::string name_;
        uint16_t surgery_, recovery_;
        std::unique_ptr<resource_ref<room>> room_;
        std::unique_ptr<resource_ref<bed>> bed_;
    public:
        patient(uint16_t id, const std::string& name, uint16_t surgery, uint16_t recovery) : id_(id), name_(name), surgery_(surgery), recovery_(recovery),
            room_(), bed_() {}

        uint16_t id() const
        {
            return id_;
        }
        const std::string& name() const
        {
            return name_;
        };
        resource_ref<room>* allocated_room() const
        {
            return room_.get();
        }
        resource_ref<bed>* allocated_bed() const
        {
            return bed_.get();
        }


        void allocate_room(std::shared_ptr<room> room, const time& when);
        void allocate_bed(std::shared_ptr<bed> bed, const time& when);

        struct less_op : public std::binary_function<std::shared_ptr<patient>, std::shared_ptr<patient>, bool>
        {
            bool operator() (const std::shared_ptr<patient>& lhs, const std::shared_ptr<patient>& rhs)
            {
                return not (lhs->room_->released() < rhs->room_->released());
            }
        };
        struct less_rec : public std::binary_function<std::shared_ptr<patient>, std::shared_ptr<patient>, bool>
        {
            bool operator() (const std::shared_ptr<patient>& lhs, const std::shared_ptr<patient>& rhs)
            {
                return not (lhs->bed_->released() < rhs->bed_->released());
            }
        };
    };

    class solution {
    public:
        solution() : rooms_(), beds_(), patients_(), start_(), finish_(), transport_room_bed_(), prepare_room_(), prepare_bed_() {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        std::vector<std::shared_ptr<room>> rooms_;
        std::vector<std::shared_ptr<bed>> beds_;
        std::vector<std::shared_ptr<patient>> patients_;
        time start_;
        time finish_;
        uint32_t transport_room_bed_;
        uint32_t prepare_room_;
        uint32_t prepare_bed_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        uint16_t n_rooms(0), n_beds(0), start(0);
        in >> n_rooms >> n_beds >> start;
        sol.start_ = start;
        sol.rooms_.clear();
        sol.rooms_.reserve(n_rooms);
        uint16_t id(0);
        std::generate_n(std::back_inserter(sol.rooms_), n_rooms, [&]()
        {
            return std::make_shared<room>(++id);
        });
        sol.beds_.clear();
        sol.beds_.reserve(n_beds);
        id  = 0;
        std::generate_n(std::back_inserter(sol.beds_), n_beds, [&]()
        {
            return std::make_shared<bed>(++id);
        });
        in >> sol.transport_room_bed_ >> sol.prepare_room_ >> sol.prepare_bed_;
        uint16_t n_patients(0);
        in >> n_patients;
        sol.patients_.clear();
        sol.patients_.reserve(n_patients);
        id = 0;
        std::generate_n(std::back_inserter(sol.patients_), n_patients, [&]()
        {
            std::string temp;
            std::getline(in, temp);
            std::getline(in, temp);
            uint32_t surgery, recovery;
            in >> surgery >> recovery;
            return std::make_shared<patient>(++id, temp, surgery, recovery);
        });
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << R"(
Patient Operating Room Recovery Room
#  Name    Room#    Begin    End   Bed#   Begin    End
------------------------------------------------------)" << std::endl;;

        for (auto patient : sol.patients_)
        {
            out << std::setw(2) << std::right << patient->id() << ' ' << std::setw(8) << std::left << patient->name() //
                << std::setw(2) << ' ' << *patient->allocated_room() << std::setw(3) << ' ' << *patient->allocated_bed() //
                << std::endl;
        }

        out << R"(
Facility Utilization
Type   # Minutes  % Used
-------------------------)" << std:: endl;;

        for (auto op_room : sol.rooms_)
        {
            out << *op_room << ' ' << std::fixed << std::setprecision(2) << std::setw(8) << std::right << op_room->utilized(
                    sol.finish_ - sol.start_) << std::endl;
        }

        for (auto r_bed : sol.beds_)
        {
            out << *r_bed << ' ' << std::fixed << std::setprecision(2) << std::setw(8) << std::right << r_bed->utilized(sol.finish_ - sol.start_) << std::endl;
        }

        return out;
    }

    template<typename R>
    struct time_cmp: public std::binary_function<std::pair<R, time>, std::pair<R, time>, bool>
    {
        bool operator()(const std::pair<R, time>& lhs, const std::pair<R, time>& rhs)
        {
            return lhs.second < rhs.second;
        }
    };

    struct event
    {
        enum class kind
        {
            none, surgery, transfer, recovery, prepare_room, prepare_bed
        };
        std::shared_ptr<patient> patient_;
        std::shared_ptr<room> room_;
        std::shared_ptr<bed> bed_;
        time when_;
        kind kind_;

        event() : patient_(), room_(), bed_(), when_(), kind_(event::kind::none) {}

        struct less : public std::binary_function<event*, event*, bool>
        {
            bool operator () (const event* lhs, const event* rhs)
            {
                return not (lhs->when_ < rhs->when_);
            }
        };
    };

    solution& solution::operator()()
    {
        std::priority_queue<std::shared_ptr<room>, std::vector<std::shared_ptr<room>>, room::less> ready_rooms(rooms_.begin(), rooms_.end());
        std::priority_queue<std::shared_ptr<bed>, std::vector<std::shared_ptr<bed>>, bed::less> ready_beds(beds_.begin(), beds_.end());

        std::priority_queue<event*, std::vector<event*>, event::less> events;

        time now = start_;
        auto pat = patients_.begin();

        while (not ready_rooms.empty())
        {
            auto op_room = ready_rooms.top();
            ready_rooms.pop();
            (*pat)->allocate_room(op_room, now);
            event* evt = new event;
            evt->when_ = (*pat)->allocated_room()->released(); // @suppress("Method cannot be resolved")
            evt->patient_ = *pat;
            evt->room_ = op_room;
            evt->kind_ = event::kind::surgery;
            events.push(evt);
            ++pat;
        }

        while (not events.empty())
        {
            auto evt = events.top();
            events.pop();
            now = evt->when_;

            switch (evt->kind_)
            {
            case event::kind::surgery:
            {
                evt->kind_ = event::kind::transfer;
                evt->when_ = now + transport_room_bed_;
                events.push(evt);
                event* prep = new event;
                prep->room_ = evt->room_;
                prep->when_ = now + prepare_room_;
                prep->kind_ = event::kind::prepare_room;
                events.push(prep);
                break;
            }

            case event::kind::transfer:
            {
                evt->kind_ = event::kind::recovery;
                auto r_bed = ready_beds.top();
                ready_beds.pop();
                evt->patient_->allocate_bed(r_bed, now);
                evt->bed_ = r_bed;
                evt->when_ = evt->patient_->allocated_bed()->released();
                events.push(evt);
                break;
            }

            case event::kind::recovery:
            {
                evt->kind_ = event::kind::prepare_bed;
                evt->when_ = now + prepare_bed_;
                events.push(evt);
                finish_ = now;
                break;
            }

            case event::kind::prepare_room:
            {
                ready_rooms.push(evt->room_);

                if (pat != patients_.end())
                {
                    auto op_room = ready_rooms.top();
                    ready_rooms.pop();
                    (*pat)->allocate_room(op_room, now);
                    event* evt = new event;
                    evt->when_ = (*pat)->allocated_room()->released(); // @suppress("Method cannot be resolved")
                    evt->patient_ = *pat;
                    evt->room_ = op_room;
                    evt->kind_ = event::kind::surgery;
                    events.push(evt);
                    ++pat;
                }
                else
                {
                    delete evt;
                }

                break;
            }

            case event::kind::prepare_bed:
            {
                ready_beds.push(evt->bed_);
                delete evt;
                break;
            }

            default:
                break;
            }
        }

        return *this;
    }

    void patient::allocate_room(std::shared_ptr<room> op_room, const time& when)
    {
        room_.reset(new resource_ref<room>(op_room, when, surgery_));
    }

    void patient::allocate_bed(std::shared_ptr<bed> rec_bed, const time& when)
    {
        bed_.reset(new resource_ref<bed>(rec_bed, when, recovery_));
    }

}

void U212::operator()() const
{
    solution sol;

    std::cin >> sol;
    std::cout << sol() << std::endl;
}
