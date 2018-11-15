#pragma once

#include <utility>
#include <iostream>
#include "disjointSet.h"

namespace math {
template<typename T>
class Kruskal {
private:
    math::DisjointSet mst_;

public:
    typedef std::pair<size_t, size_t> Edge;

    typedef std::pair<T, Edge> Line;

    explicit Kruskal(size_t v) : mst_()
    {
        mst_.initSet(v);
    }

    T operator()(std::vector <Line>& lines);
};
}
