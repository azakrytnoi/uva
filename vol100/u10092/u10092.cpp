#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10092.h"

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
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10092 instance;
    instance();
}

namespace {

    struct node_t
    {
        int32_t x_, y_, v_;
        int32_t next_;
    };

    class solution_t {
    public:
        solution_t() : solved_(), nk_(), nr_(), count_(), edges_(), results_(), heads_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool() const
        {
            return nk_ > 0 && nr_ > 0;
        }
        solution_t& operator()();

    private:
        bool solved_;
        int32_t nk_, nr_;
        int32_t count_;
        std::vector<node_t> edges_;
        std::vector<std::vector<int32_t>> results_;
        std::vector<int32_t> heads_;

        void addEdge(int32_t x, int32_t y, int32_t v)
        {
            edges_.push_back({x, y, v, heads_[x]});
            heads_[x] = edges_.size() - 1;
            edges_.push_back({y, x, 0, heads_[y]});
            heads_[y] = edges_.size() - 1;
        }

        int32_t maxflow(int32_t start, int32_t end)
        {
            auto flow (0);

            std::vector<int32_t> prev(1100);
            std::vector<int32_t> record(1100);

            while (true)
            {
                std::vector<int32_t> dis(1100);
                dis[start] = std::numeric_limits<int32_t>::max();
                std::queue<int32_t> work;
                work.push(start);

                while (not work.empty())
                {
                    auto x (work.front());
                    work.pop();

                    for (auto i = heads_[x]; i != -1; i = edges_[i].next_)
                    {
                        auto y (edges_[i].y_);

                        if (dis[y] == 0 && edges_[i].v_ > 0)
                        {
                            prev[y] = x, record[y] = i;
                            dis[y] = std::min(dis[x], edges_[i].v_);
                            work.push(y);
                        }
                    }

                    if (dis[end])
                    {
                        break;
                    }
                }

                if (dis[end] == 0)
                {
                    break;
                }

                flow += dis[end];

                for (auto x = end; x != start; x = prev[x])
                {
                    auto ri = record[x];
                    edges_[ri].v_ -= dis[end];
                    edges_[ri ^ 1].v_ += dis[end];
                }

            }

            return flow;
        }
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.results_.clear();

        if (in >> sol.nk_ >> sol.nr_ && sol)
        {
            sol.edges_.clear();
            sol.edges_.reserve(500005);
            sol.heads_.clear();
            sol.heads_.resize(110, -1);
            auto start(0), end(sol.nk_ + sol.nr_);
            sol.count_ = 0;

            for (auto i = 1; i <= sol.nk_; ++i)
            {
                int32_t x;
                in >> x;
                sol.addEdge(sol.nr_ + i, end, x);
                sol.count_ += x;
            }

            for (auto i = 1; i <= sol.nr_; ++i)
            {
                int32_t x, y;
                in >> x;

                while (x--)
                {
                    in >> y;
                    sol.addEdge(i, sol.nr_ + y, 1);
                }

                sol.addEdge(start, i, 1);

            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.solved_)
        {
            out << 1 << std::endl;

            for (auto i = 0; i < sol.nk_; ++i)
            {
                for (auto j = 0u; j < sol.results_[i].size(); ++j)
                {
                    if (j != 0)
                    {
                        out << ' ';
                    }

                    out << sol.results_[i][j];
                }

                out << std::endl;
            }
        }
        else
        {
            out << 0 << std::endl;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        solved_ = false;
        auto flow(maxflow(0, nk_ + nr_));

        if (flow == count_)
        {
            solved_ = true;
            results_.clear();
            results_.resize(nk_);

            for (auto i = 1; i <= nr_; ++i)
            {
                for (auto j = heads_[i]; j != -1; j = edges_[j].next_)
                {
                    if (edges_[j].v_ == 0 && edges_[j].y_ > nr_)
                    {
                        results_[edges_[j].y_ - nr_ - 1].push_back(i);
                        break;
                    }
                }
            }
        }

        return *this;
    }

}

void U10092::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
