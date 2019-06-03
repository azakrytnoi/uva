#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdint>

namespace math {
    struct disjointSet_t
    {
        uint64_t sets_;
        std::vector<uint64_t> pAddress_;
        std::vector<uint64_t> numVertices_;

        disjointSet_t() : sets_(0), pAddress_(), numVertices_() {}

        void initSet(size_t n)
        {
            sets_ = n;
            pAddress_.clear();
            numVertices_.clear();
            pAddress_.reserve(n + 1);
            numVertices_.reserve(n + 1);
            uint64_t i = 0;
            std::generate_n(std::back_inserter(pAddress_), n + 1, [&i]()
            {
                return i++;
            });
            std::generate_n(std::back_inserter(numVertices_), n + 1, []()
            {
                return 1;
            });
        }

        uint64_t findSet(uint64_t x)
        {
            if (x == pAddress_[x])
            {
                return x;

            }
            else
            {
                return pAddress_[x] = findSet(pAddress_[x]);
            }
        }

        uint64_t setSize(uint64_t x)
        {
            return numVertices_[findSet(x)];
        }

        bool isSameSet(uint64_t x, uint64_t y)
        {
            if (findSet(x) == findSet(y))
            {
                return true;

            }
            else
            {
                return false;
            }
        }

        void unionSet(uint64_t x, uint64_t y)
        {
            uint64_t xRoot = findSet(x);
            uint64_t yRoot = findSet(y);

            if (!isSameSet(xRoot, yRoot))
            {
                numVertices_[yRoot] += numVertices_[xRoot];
                pAddress_[xRoot] = yRoot;
                sets_--;
            }
        }
    };
}
