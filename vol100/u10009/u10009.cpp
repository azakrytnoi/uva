#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10009.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <deque>
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
    U10009 instance;
    instance();
}

namespace {
    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), M_(), start_(), end_(), route_(), graph_() {}

        operator bool()
        {
            return M_ == 0 ? N_-- > 0 : true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t N_, M_;
        std::string start_, end_;
        std::map<std::string, std::string> route_;
        std::map<std::string, std::vector<std::string>> graph_;

        void printPath(const std::string& end, const std::string& start, std::ostream& out) const
        {
            if (start == end)
            {
                out << start[0];
            }
            else
            {
                printPath(route_.find(end)->second, start, out);
                out << end[0];
            }
        };
    };

    solution_t& solution_t::operator()()
    {
        route_.clear();
        std::map<std::string, int32_t> visited;
        auto bfs = [&]()
        {
            std::deque<std::string> work;
            visited[start_] = 0;
            work.push_back(start_);

            while (not work.empty())
            {
                auto top = work.front();
                work.pop_front();

                if (top == end_)
                {
                    break;
                }

                for (auto current = graph_[top].begin(); current != graph_[top].end(); ++current)
                {
                    if (visited.find(*current) == visited.end())
                    {
                        visited[*current] = visited[top] + 1;
                        work.push_back(*current);
                        route_[*current] = top;
                    }
                }
            }
        };
        bfs();
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        if (sol.N_ > 0)
        {
            if (sol.M_ == 0)
            {
                sol.graph_.clear();
                size_t n(0);
                in >> n >> sol.M_;

                while (n--)
                {
                    std::string start, end;
                    in >> start >> end;
                    sol.graph_[start].push_back(end);
                    sol.graph_[end].push_back(start);
                }
            }

            --sol.M_;
            in >> sol.start_ >> sol.end_;
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        sol.printPath(sol.end_, sol.start_, out);

        if (sol.M_ == 0)
        {
            out << std::endl;
        }

        return out;
    }
}

void U10009::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
