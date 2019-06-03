#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u405.h"

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
    U405 instance;
    instance();
}

namespace {

    class ORName_t {
    public:
        ORName_t() : country_(), admd_(), prmd_(), organization_() {}
        ORName_t(const ORName_t& rhs) : country_(rhs.country_), admd_(rhs.admd_), prmd_(rhs.prmd_),
            organization_(rhs.organization_) {}
        ORName_t(ORName_t&& rhs) : country_(std::move(rhs.country_)), admd_(std::move(rhs.admd_)), prmd_(std::move(rhs.prmd_)),
            organization_(std::move(rhs.organization_)) {}

        bool operator == (const ORName_t& rhs) const;

        friend std::istream& operator>>(std::istream& in, ORName_t& orName);

    private:
        std::string country_, admd_, prmd_, organization_;
    };

    class solution_t {
    public:
        solution_t() : mta_(), messages_(), seq_no_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return true;
        }
        solution_t& operator()();

    private:
        enum class delivery_t
        {
            none, delivered, unable, circle
        };
        std::map<std::string, std::vector<std::pair<std::string, ORName_t>>> mta_;
        std::vector<std::tuple<std::string, ORName_t, std::pair<delivery_t, std::vector<std::string>>>> messages_;
        size_t seq_no_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.mta_.clear();
        sol.messages_.clear();
        size_t mta_n(0);

        if (in >> mta_n)
        {
            std::vector<std::pair<std::string, std::vector<std::pair<std::string, ORName_t>>>> input;
            input.reserve((mta_n));
            std::generate_n(std::back_inserter(input), mta_n, [&]()
            {
                std::pair<std::string, std::vector<std::pair<std::string, ORName_t>>> mta;
                size_t routes(0);
                in >> mta.first >> routes;
                mta.second.reserve(routes);
                std::generate_n(std::back_inserter(mta.second), routes, [&]()
                {
                    std::pair<std::string, ORName_t> route;
                    in >> route.first >> route.second;
                    return route;
                });
                return mta;
            });
            sol.mta_.insert(input.begin(), input.end());
            in >> mta_n;
            sol.messages_.reserve(mta_n);
            std::generate_n(std::back_inserter(sol.messages_), mta_n, [&]()
            {
                std::tuple<std::string, ORName_t, std::pair<solution_t::delivery_t, std::vector<std::string>>> message;
                in >> std::get<0>(message) >> std::get<1>(message);
                return message;
            });
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, ORName_t& name)
    {
        in >> name.country_ >> name.admd_ >> name.prmd_ >> name.organization_;
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        out << "Scenario # " << sol.seq_no_ << std::endl;
        size_t n(0);

        for (auto& message : sol.messages_)
        {
            auto& trace = std::get<2>(message);
            out << (++n) << " -- ";

            switch (trace.first)
            {
            case solution_t::delivery_t::delivered:
                out << "delivered to " << trace.second.back();
                break;

            case solution_t::delivery_t::unable:
                out << "unable to route at " << trace.second.back();
                break;

            case solution_t::delivery_t::circle:
                out << "circular routing detected by " << trace.second.back();
                break;

            default:
                out << "unprocessed, shouldn't happen";
                break;
            }

            out << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        seq_no_++;

        for (auto& message : messages_)
        {
            auto& trace = std::get<2>(message);
            auto mta_name = std::get<0>(message);
            auto& dstName = std::get<1>(message);

            while (trace.first == delivery_t::none)
            {
                if (std::find(trace.second.begin(), trace.second.end(), mta_name) != trace.second.end())
                {
                    trace.first = delivery_t::circle;
                    trace.second.push_back(mta_name);
                    break;
                }

                trace.second.push_back(mta_name);
                auto& routes = mta_.find(mta_name)->second;

                trace.first = delivery_t::unable;

                for (auto& route : routes)
                {
                    if (route.second == dstName)
                    {
                        if (route.first == mta_name)
                        {
                            trace.first = delivery_t::delivered;
                        }
                        else
                        {
                            trace.first = delivery_t::none;
                            mta_name = route.first;
                        }

                        break;
                    }
                }
            }
        }

        return *this;
    }

    bool ORName_t::operator ==(const ORName_t& rhs) const
    {
        if (country_ == "*" || country_ == rhs.country_)
        {
            if (admd_ == "*" || admd_ == rhs.admd_)
            {
                if (prmd_ == "*" || prmd_ == rhs.prmd_)
                {
                    if (organization_ == "*" || organization_ == rhs.organization_)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

}

void U405::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
