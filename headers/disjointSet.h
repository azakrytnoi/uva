#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdint>

namespace math {
	struct DisjointSet {
		uint64_t sets;
		std::vector<uint64_t> pAddress;
		std::vector<uint64_t> numVertices;

		DisjointSet() : sets(0), pAddress(), numVertices() {}

		void initSet(uint64_t n)
		{
			sets = n;
			pAddress.clear();
			numVertices.clear();
			pAddress.reserve(n + 1);
			numVertices.reserve(n + 1);
			uint64_t i = 0;
			std::generate_n(std::back_inserter(pAddress), n + 1, [&i]() {
				return i++;
			});
			std::generate_n(std::back_inserter(numVertices), n + 1, []() {
				return 1;
			});
		}

		uint64_t findSet(uint64_t x)
		{
			if (x == pAddress[x])
				return x;
			else
				return pAddress[x] = findSet(pAddress[x]);
		}

		uint64_t setSize(uint64_t x)
		{
			return numVertices[findSet(x)];
		}

		bool isSameSet(uint64_t x, uint64_t y)
		{
			if (findSet(x) == findSet(y))
				return true;
			else
				return false;
		}

		void unionSet(uint64_t x, uint64_t y)
		{
			uint64_t xRoot = findSet(x);
			uint64_t yRoot = findSet(y);

			if (!isSameSet(xRoot, yRoot)) {
				numVertices[yRoot] += numVertices[xRoot];
				pAddress[xRoot] = yRoot;
				sets--;
			}
		}
	};
}