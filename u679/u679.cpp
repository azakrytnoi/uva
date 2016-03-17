#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u679.h"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

namespace {
class node {
public:
    node() : id_(++id_gen), left_(nullptr), right_(nullptr), flag_(false) {}
    node(const node& rhs) = delete;

    node& operator = (const node& rhs) = delete;

    ~node()
    {
        delete left_;
        delete right_;
    }

    int id() const
    {
        return id_;
    }

    node*& left()
    {
        return left_;
    }

    node*& right()
    {
        return right_;
    }

    node* traverse()
    {
        flag_ = !flag_;
        return !flag_ ? right_ : left_;
    }

    static int id_gen;

private:
    int id_;
    node* left_;
    node* right_;
    bool flag_;
};

int node::id_gen;
}

U679::U679()
{
}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U679 instance;
    instance();
}
void U679::operator()()
{
    int n_cases;
    while ((std::cin >> n_cases) && (n_cases > 0)) {
        while (n_cases-- > 0) {
            int D, I;
            std::cin >> D >> I;
            node::id_gen = 0;
            node* root = new node();
            std::map < int, std::vector<node*>> nodes;
            nodes[1].push_back(root);
            for (int i = 2; i <= D; ++i) {
                std::for_each(nodes[i - 1].begin(), nodes[i - 1].end(), [&](node * n) {
                    n->left() = new node();
                    n->right() = new node();
                    nodes[i].push_back(n->left());
                    nodes[i].push_back(n->right());
                });
            }
            node* current = nullptr;
            for (int i = 1; i <= I; ++i) {
                current = root;
                node * next = current;
                while (next != nullptr) {
                    current = next;
                    next = current->traverse();
                }
            }
            std::cout << current->id() << std::endl;
            nodes.clear();
            delete root;
        }
    }
}