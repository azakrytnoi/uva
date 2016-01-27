#include "U200.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

U200::U200()
{
}


U200::~U200()
{
}

std::string U200::operator()()
{
	std::vector<std::string> index;
	std::string ordering;
	{
		std::string line;
		while (std::getline(std::cin, line))
		{
			char ch = line[0];
			if (ch != '#')
			{
				index.push_back(line);
				if (ordering.find(ch) == std::string::npos)
				{
					ordering.push_back(ch);
				}
			}
			else
			{
				break;
			}
		}
	}
	{
		for each (char start_ch in ordering)
		{
			std::vector<std::string> selected;
			std::copy_if(index.begin(), index.end(), std::back_inserter(selected), [&](std::string &line) { return line[0] == start_ch; });
			if (selected.size() > 1)
			{
				size_t idx = 1;
				while (!selected.empty())
				{
					std::vector<std::string> working;
					std::copy_if(selected.begin(), selected.end(), std::back_inserter(working), [&](std::string& line) { return line.size() > idx; });
					for (size_t i = 1; i < working.size(); ++i)
					{
						char prev_ch = working[i - 1][idx];
						char curr_ch = working[i][idx];
						if (prev_ch == curr_ch) continue;
						if (ordering.find(curr_ch) == std::string::npos)
						{
							const size_t prev_pos = ordering.find(prev_ch);
							if (prev_pos != std::string::npos)
							{
								ordering.insert(prev_pos + 1, 1, curr_ch);
							}
						}
					}
					idx++;
					selected = working;
				}
			}
		}
	}
	return ordering;
}
