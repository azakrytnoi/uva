#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)

#include "u908.h"

#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>
#include <iterator>
#include <numeric>

U908::U908()
{
}

U908::~U908()
{
}

namespace
{
typedef std::pair<int, int> Edge;
typedef std::pair<int, Edge> Line;

std::istream& operator >> (std::istream& in, Line& line)
{
    in >> line.second.first >> line.second.second >> line.first;
    return in;
}

struct DisjointSet {
    int sets;
    std::vector<int> pAddress;
    std::vector<int> numVertices;

    void initSet(int n)
    {
        sets = n;
        pAddress.clear();
        numVertices.clear();
        pAddress.reserve(n + 1);
        numVertices.reserve(n + 1);
        int i = 0;
        std::generate_n(std::back_inserter(pAddress), n + 1, [&i]() {
            return i++;
        });
        std::generate_n(std::back_inserter(numVertices), n + 1, []() {
            return 1;
        });
    }

    int findSet(int x)
    {
        if (x == pAddress[x])
            return x;
        else
            return pAddress[x] = findSet(pAddress[x]);
    }

    int setSize(int x)
    {
        return numVertices[findSet(x)];
    }

    bool isSameSet(int x, int y)
    {
        if (findSet(x) == findSet(y))
            return true;
        else
            return false;
    }

    void unionSet(int x, int y)
    {
        int xRoot = findSet(x);
        int yRoot = findSet(y);

        if (!isSameSet(xRoot, yRoot)) {
            numVertices[yRoot] += numVertices[xRoot];
            pAddress[xRoot] = yRoot;
            sets--;
        }
    }
};

class Kruskal
{
private:
    DisjointSet mst_;

public:
    explicit Kruskal(int v)
    {
        mst_.initSet(v);
    }

    int operator()(std::vector <Line>& lines);
};

int Kruskal::operator()(std::vector <Line>& lines)
{
    int total = 0;

    sort(lines.begin(), lines.end(), [](auto a, auto b) -> bool { return a.first < b.first; });
    std::for_each(lines.begin(), lines.end(), [this, &total](auto line) {
        int cost = line.first;
        int origin = line.second.first;
        int destination = line.second.second;
        if (!mst_.isSameSet(origin, destination)) {
            total += cost;
            mst_.unionSet(origin, destination);
        }
    });
    return total;
}
}

void U908::operator()()
{
    int V;

    auto readLine = []() -> Line {
        Line line;
        std::cin >> line;
        return line;
    };
    std::vector <Line> lines;
    while (std::cin >> V) {
        Kruskal kruskal(V);
        int K, M, total(0);
        lines.clear();
        for (int i = 0; i < V - 1; i++) {
            total += readLine().first;
        }
        std::cout << total << std::endl;
        std::cin >> K;
        std::generate_n(std::back_inserter(lines), K, readLine);
        std::cin >> M;
        std::generate_n(std::back_inserter(lines), M, readLine);
        std::cout << kruskal(lines) << std::endl;
        std::cout << std::endl;
    }
}
