#include "U10305.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>

namespace
{
	struct task
	{
		int id;
		std::vector<int> predessors;
		bool executed;

		task() : id(0), executed(false) {}
		task(int id) : id(id), executed(false) {}
	};

	std::ostream& operator << (std::ostream& out, const task& t)
	{
		out << t.id;
		return out;
	}
}

U10305::U10305()
{
}


U10305::~U10305()
{
}

std::string U10305::operator()()
{
	std::vector<task> tasks;
	{
		int n, m;
		std::cin >> n >> m;
		tasks.reserve(n);
		std::generate_n(std::back_inserter(tasks), n, [&]() { return task(tasks.size() + 1); });
		while (std::cin >> n >> m)
		{
			if (n == 0 && m == 0) break;
			tasks[m - 1].predessors.push_back(n);
		}
	}
	std::stringstream out;
	std::vector<int> done_tasks;
	done_tasks.reserve(tasks.size());
	std::for_each(tasks.begin(), tasks.end(), [&](task& t) { if (t.predessors.empty()) { done_tasks.push_back(t.id); t.executed = true; } });
	if (done_tasks.empty())
	{
		return "No solution";
	}
	else
	{
		std::vector<int> current;
		while (done_tasks.size() < tasks.size())
		{
			current.clear();
			current.reserve(tasks.size());
			std::for_each(tasks.begin(), tasks.end(), [&](task& t)
			{
				if (!t.executed)
				{
					bool ready(true);
					for each (int idx in t.predessors)
					{
						ready &= tasks[idx - 1].executed;
						if (!ready)
						{
							break;
						}
					}
					if (ready)
					{
						current.push_back(t.id);
					}
				}
			});
			std::_For_each(current.begin(), current.end(), [&](int idx) {done_tasks.push_back(idx); tasks[idx - 1].executed = true; });
		}
		std::ostream_iterator<int> oit(out, " ");
		std::copy(done_tasks.begin(), done_tasks.end(), oit);
	}
	return out.str();
}
