#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

namespace math {
	struct DisjointSet {
		int sets;
		std::vector<int> pAddress;
		std::vector<int> numVertices;

		DisjointSet() : sets(0), pAddress(), numVertices() {}

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
}