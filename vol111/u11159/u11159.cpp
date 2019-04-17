#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11159.h"

#include <iostream>
#include <iomanip>
#include <vector>
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
    U11159 instance;
    instance();
}

namespace {

    template<size_t N, size_t M>
    class solution {
        int n_, m_;
        std::vector<int> a_;
        std::vector<int> b_;
        std::vector<int> pn_;
        std::vector<int> pm_;
        std::vector<int> q_;
        std::vector<int> dist_;
        std::vector<std::vector<int>> g_;
        int result_;
        int caseNo_;

        static const int INFINITY_;
    public:
        solution() : n_(0), m_(0), a_(), b_(), pn_(), pm_(), q_(), dist_(), g_(), result_(0), caseNo_(0)
        {
            a_.reserve(N);
            b_.reserve(N);
            g_.reserve(N);
            pn_.reserve(N);
            pm_.reserve(N);
            q_.resize(M);
            dist_.resize(N);
        }

        template<size_t NN, size_t MM>
        friend std::istream& operator >> (std::istream& in, solution<NN, MM>& sol);
        template<size_t NN, size_t MM>
        friend std::ostream& operator << (std::ostream& out, solution<NN, MM>& sol);

        solution& operator()();

    private:
        void Hopcroft_Karp();
        bool DFS(int v);
        bool BFS();
    };

    template<size_t N, size_t M>
    const int solution<N, M>::INFINITY_ = 1000000000;

    template<size_t N, size_t M>
    solution<N, M>& solution<N, M>::operator()()
    {
        for (int i = 1; i <= n_; ++i)
        {
            for (int j = 1; j <= m_; ++j)
            {
                if (a_[i] != 0 && b_[j] % a_[i] == 0)
                {
                    g_[i].push_back(j + n_);
                }

                if (a_[i] == 0 && b_[j] == 0)
                {
                    g_[i].push_back(j + n_);
                }
            }
        }

        Hopcroft_Karp();
        return *this;
    }

    template<size_t N, size_t M>
    void solution<N, M>::Hopcroft_Karp()
    {
        result_ = 0;
        pn_.clear();
        pm_.clear();
        pn_.resize(n_ + m_ + 2);
        pm_.resize(n_ + m_ + 2);

        while (BFS())
        {
            for (int i = 1; i <= n_; ++i)
            {
                if (pn_[i] == 0)
                {
                    if (DFS(i))
                    {
                        ++result_;
                    }
                }
            }
        }
    }

    template<size_t N, size_t M>
    bool solution<N, M>::DFS(int v)
    {
        if (v != 0)
        {
            for (int j = 0; j < int(g_[v].size()); ++j)
            {
                int u = g_[v][j];

                if (dist_[pm_[u]] == dist_[v] + 1)
                {
                    if (DFS(pm_[u]))
                    {
                        pm_[u] = v;
                        pn_[v] = u;
                        return true;
                    }
                }
            }

            dist_[v] = INFINITY_;
            return false;
        }

        return true;
    }

    template<size_t N, size_t M>
    bool solution<N, M>::BFS()
    {
        int b (0), en (0);

        for (int i = 1; i <= n_; ++i)
        {
            if (pn_[i] == 0)
            {
                dist_[i] = 0;
                q_[en++] = i;

            }
            else
            {
                dist_[i] = INFINITY_;
            }
        }

        dist_[0] = INFINITY_;

        while (b < en)
        {
            int v = q_[b++];

            if (dist_[v] < dist_[0])
            {
                for (int j = 0; j < int(g_[v].size()); ++j)
                {
                    int u = g_[v][j];

                    if (dist_[pm_[u]] == INFINITY_)
                    {
                        dist_[pm_[u]] = dist_[v] + 1;
                        q_[en++] = pm_[u];
                    }
                }
            }
        }

        return dist_[0] != INFINITY_;
    }

    typedef solution<333, 100000> u11159Solution;

    template<size_t N, size_t M>
    std::istream& operator >> (std::istream& in, solution<N, M>& sol)
    {
        auto readArray = [&]()
        {
            int tmp;
            in >> tmp;
            return tmp;
        };
        std::cin >> sol.n_;
        sol.a_.clear();
        sol.a_.push_back(0);
        std::generate_n(std::back_inserter(sol.a_), sol.n_, readArray);
        std::cin >> sol.m_;
        sol.b_.clear();
        sol.b_.push_back(0);
        std::generate_n(std::back_inserter(sol.b_), sol.m_, readArray);
        sol.g_.clear();
        sol.g_.resize(sol.n_ + sol.m_);
        return in;
    }

    template<size_t N, size_t M>
    std::ostream& operator << (std::ostream& out, solution<N, M>& sol)
    {
        std::cout << "Case " << (++sol.caseNo_) << ": " << sol.result_;
        return out;
    }

}

void U11159::operator()() const
{
    int T;
    std::cin >> T;
    u11159Solution sol;

    while (T--)
    {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
