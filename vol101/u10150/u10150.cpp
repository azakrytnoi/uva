#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10150.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <queue>
#include <list>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10150 instance;
    instance();
}

namespace {

    class solution {
        std::map<size_t, std::list<std::string>> dictionary_;
        std::string a_;
        std::string b_;
        size_t a_idx_;
        size_t b_idx_;
        std::vector<int64_t> path_;
        std::vector<std::string> doublets_;
        bool have_solution_;
    public:
        solution() : dictionary_(), a_(), b_(), a_idx_(), b_idx_(), path_(), doublets_(), have_solution_(false) {}

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        bool operator () (std::istream& in);

    private:
        void print_path(std::ostream& out, size_t start, size_t end) const;
        bool is_doublets(size_t u, size_t v);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::string word;

        while (std::getline(in, word) && !word.empty())
        {
            sol.dictionary_[word.length()].push_back(word);
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.have_solution_)
        {
            sol.print_path(out, sol.a_idx_, sol.b_idx_);
        }
        else
        {
            out << "No solution." << std::endl;
        }

        return out;
    }

    void solution::print_path(std::ostream& out, size_t start, size_t end) const
    {
        if (start != end)
        {
            print_path(out, start, path_[end]);
        }

        out << doublets_[end] << std::endl;
    }

    bool solution::operator ()(std::istream& in)
    {
        have_solution_ = false;
        in >> a_ >> b_;

        if (a_.length() == b_.length())
        {
            doublets_.assign(dictionary_[a_.length()].begin(), dictionary_[a_.length()].end());
            a_idx_ = std::find(doublets_.begin(), doublets_.end(), a_) - doublets_.begin();
            b_idx_ = std::find(doublets_.begin(), doublets_.end(), b_) - doublets_.begin();

            if (a_idx_ < doublets_.size() && b_idx_ < doublets_.size())
            {
                path_.clear();
                path_.reserve(doublets_.size());
                std::generate_n(std::back_inserter(path_), doublets_.size(), []()
                {
                    return -1;
                });
                path_[a_idx_] = a_idx_;
                std::queue<size_t> path_q;
                path_q.push(a_idx_);

                while (path_q.size() && path_q.front() != b_idx_)
                {
                    size_t u = path_q.front();
                    path_q.pop();

                    for (size_t v = 0; v < doublets_.size(); v++)
                    {
                        if (u != v && path_[v] < 0 && is_doublets(u, v))
                        {
                            path_[v] = u;
                            path_q.push(v);
                        }
                    }
                }

                have_solution_ = path_[b_idx_] != -1;
            }
        }

        return (bool)in;
    }

    bool solution::is_doublets(size_t u, size_t v)
    {
        size_t diff (0);

        for (auto itu = doublets_[u].begin(), itv = doublets_[v].begin(); diff < 2 && itu != doublets_[u].end(); ++itu, ++itv)
        {
            diff += (*itu) != (*itv);
        }

        return diff == 1;
    }

}

void U10150::operator()() const
{
    solution sol;
    std::cin >> sol;

    while (sol(std::cin))
    {
        std::cout << sol << std::endl;
    }
}
