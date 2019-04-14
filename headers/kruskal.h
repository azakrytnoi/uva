#pragma once

#include <utility>
#include <iostream>

#include "disjointSet.h"

namespace math {
    template<typename T>
    class kruskal_t {
    private:
        math::disjointSet_t mst_;

    public:
        typedef std::pair<size_t, size_t> edge_t;

        typedef std::pair<T, edge_t> line_t;

        explicit kruskal_t(size_t v) : mst_()
        {
            mst_.initSet(v);
        }

        T operator()(std::vector <line_t>& lines);
    };
}
