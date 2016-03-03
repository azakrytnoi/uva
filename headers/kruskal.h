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
			typedef std::pair<int, int> Edge;

			typedef std::pair<T, Edge> Line;

			explicit Kruskal(int v) : mst_()
			{
				mst_.initSet(v);
			}

			T operator()(std::vector <Line>& lines);
		};


}