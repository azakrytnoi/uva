#include "U146.h"

#include <string>
#include <iostream>
#include <algorithm>

U146::U146()
{
}


U146::~U146()
{
}
void U146::operator()()
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		if ("#" == line)
		{
			break;
		}

		if (std::next_permutation(line.begin(), line.end())) {
			std::cout << line << std::endl;
		}
		else
		{
			std::cout << "No Successor" << std::endl;
		}
	}
}
