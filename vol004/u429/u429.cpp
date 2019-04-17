#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u429.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <queue>
#include <memory>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U429 instance;
    instance();
}

namespace {

    template<typename vdata>
    struct vertex_t
    {
        vdata id_;
        std::vector<std::shared_ptr<vertex_t>> adjacent_;
        vertex_t (const vdata& id) : id_(id), adjacent_() {}
    };

    template<typename vdata>
    class graph_t {
        typedef std::map<vdata, std::shared_ptr<vertex_t<vdata>>> vmap_t;
        typedef std::pair<vdata, std::shared_ptr<vertex_t<vdata>>> vpair_t;
    public:
        graph_t() : vertexMap_(), allVertexes_() {}
        std::shared_ptr<vertex_t<vdata>> addVertex (const vdata& v);
        void addEdge (const vdata& begin, const vdata& end, bool directed = false);
        std::shared_ptr<vertex_t<vdata>> firstVertex() const;
        int32_t bfs (const vdata& begin, const vdata& end);

        void reset()
        {
            vertexMap_.clear();
            allVertexes_.clear();
        }

    private:
        vmap_t vertexMap_;
        std::vector<std::shared_ptr<vertex_t<vdata>>> allVertexes_;

        std::shared_ptr<vertex_t<vdata>> getVertex (const vdata& v);
    };

    template<typename vdata>
    std::shared_ptr<vertex_t<vdata>> graph_t<vdata>::addVertex (const vdata& v)
    {
        auto newv = std::make_shared<vertex_t<vdata>>(v);
        allVertexes_.push_back (newv);
        vertexMap_.insert (vpair_t (v, newv));
        return newv;
    }

    template<typename vdata>
    void graph_t<vdata>::addEdge (const vdata& begin, const vdata& end, bool directed)
    {
        auto v = getVertex (begin);
        auto w = getVertex (end);
        v->adjacent_.push_back (w);

        if (!directed)
        {
            w->adjacent_.push_back (v);
        }
    }

    template<typename vdata>
    std::shared_ptr<vertex_t<vdata>> graph_t<vdata>::getVertex (const vdata& v)
    {
        auto it = vertexMap_.find (v);

        if (it == vertexMap_.end())
        {
            return addVertex (v);
        }

        return it->second;
    }

    template<typename vdata>
    std::shared_ptr<vertex_t<vdata>> graph_t<vdata>::firstVertex() const
    {
        return allVertexes_.front();
    }

    template<typename vdata>
    int32_t graph_t<vdata>::bfs (const vdata& begin, const vdata& end)
    {
        std::queue<std::pair<std::shared_ptr<vertex_t<vdata>>, int>> work;
        auto beginning = getVertex (begin);
        std::map<std::string, bool> taken;

        work.push (std::make_pair(beginning, 0));

        while (!work.empty())
        {
            auto temp (work.front());
            work.pop();

            if (temp.first->id_ == end)
            {
                return temp.second;
            }

            for (auto& adj : temp.first->adjacent_)
            {
                if (taken[adj->id_] == false)
                {
                    taken[adj->id_] = true;
                    work.push (std::make_pair(adj, temp.second + 1));
                }
            }
        }

        return -1;
    }

    class solution_t {
    public:
        solution_t() : n_(std::numeric_limits<size_t>::max()), dictionary_(), graph_(), words_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return --n_ > 0;
        }
        solution_t& operator()();

    private:
        size_t n_;
        std::vector<std::string> dictionary_;
        graph_t<std::string> graph_;
        std::vector<std::tuple<std::string, std::string, int32_t>> words_;
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        sol.words_.clear();

        if (sol.n_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.n_;
            in.ignore();
        }

        std::string line;

        while (std::getline(in, line) && line != "*")
        {
            if (not line.empty())
            {
                sol.dictionary_.push_back(line);
            }
        }

        while (std::getline(in, line) && not line.empty())
        {
            std::stringstream temp(line);
            std::tuple<std::string, std::string, int32_t> pair;
            temp >> std::get<0>(pair) >> std::get<1>(pair);
            sol.words_.push_back(pair);
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        std::for_each(sol.words_.begin(), sol.words_.end(), [&](const std::tuple<std::string, std::string, int32_t>& pair)
        {
            out << std::get<0>(pair) << " " << std::get<1>(pair) << " " << std::get<2>(pair) << std::endl;
        });
        return out;
    }

    solution_t& solution_t::operator()()
    {
        graph_.reset();

        for (auto i = dictionary_.begin(); i != dictionary_.end(); ++i)
        {
            for (auto j = i + 1; j != dictionary_.end(); ++j)
            {
                if (i->size() != j->size())
                {
                    continue;
                }

                size_t diff_cnt(0);
                auto ik = i->begin();
                auto jk = j->begin();

                for (; diff_cnt < 2 && ik != i->end(); ++ik, ++jk)
                {
                    if (*ik != *jk)
                    {
                        ++diff_cnt;
                    }
                }

                if (diff_cnt < 2)
                {
                    graph_.addEdge (*i, *j);
                }
            }
        }

        for (auto& pair : words_)
        {
            std::get<2>(pair) = graph_.bfs(std::get<0>(pair), std::get<1>(pair));
        }

        return *this;
    }

}

void U429::operator()() const
{
    solution_t sol;

    do
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
    while (sol);
}
