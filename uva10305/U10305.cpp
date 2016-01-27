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
	std::vector<task> done_tasks;
	done_tasks.reserve(tasks.size());
	std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(done_tasks), [](task t) { return t.predessors.empty(); });
	if (done_tasks.empty())
	{
		return "No solution";
	}
	else
	{
		while (done_tasks.size() < tasks.size())
		{
			std::for_each(done_tasks.begin(), done_tasks.end(), [&](task t) { tasks[t.id - 1].executed = true; });
			std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(done_tasks), [&](task t) -> bool 
			{
				if (!t.executed)
				{
					bool ready(true);
					for each (int tt in t.predessors)
					{
						ready &= tasks[tt - 1].executed;
						if (!ready)
						{
							break;
						}
					}
					return ready;
				}
				return false;
			});
		}
		std::ostream_iterator<task> oit(out, " ");
		std::copy(done_tasks.begin(), done_tasks.end(), oit);
	}
	return out.str();
}
