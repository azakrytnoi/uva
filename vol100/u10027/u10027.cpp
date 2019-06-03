#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10027.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>
#include <regex>
#include <list>
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10027 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), initial_(), dictionary_(), language_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        std::string initial_;
        std::map<std::string, std::list<std::string>> dictionary_;
        std::set<std::string> language_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
            in.ignore(2);
        }

        if (std::getline(in, sol.initial_))
        {
            sol.language_.clear();
            sol.dictionary_.clear();
            sol.initial_.erase(sol.initial_.begin());
            sol.initial_.erase(sol.initial_.end() - 1);
            std::string temp;
            std::regex pattern("\"([^\"]+)\"->\"([^\"]+)\"");

            while (std::getline(in, temp) && not temp.empty())
            {
                std::smatch match;

                if (std::regex_search(temp, match, pattern))
                {
                    sol.dictionary_[match[1]].push_back(match[2]);
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.language_.size() > 1000)
        {
            out << "Too many" << std::endl;
        }
        else
        {
            out << sol.language_.size() << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        std::queue<std::string> work;
        work.push(initial_);
        language_.insert(initial_);

        while (not work.empty() && language_.size() < 1000)
        {
            auto item = work.front();
            work.pop();

            for (auto& sub : dictionary_)
            {
                std::regex pattern("(.*)(" + sub.first + ")(.*)");

                if (std::regex_match(item, pattern))
                {
                    for (auto& next : sub.second)
                    {
                        std::string result;
                        std::regex_replace(std::back_inserter(result), item.begin(), item.end(), pattern, "$1" + next + "$3");
                        language_.insert(result);
                        work.push(result);
                    }
                }
            }
        }

        return *this;
    }

}

void U10027::operator()() const
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
