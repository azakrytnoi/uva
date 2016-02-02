#include "U482.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

U482::U482()
{
}

U482::~U482()
{
}

namespace
{
	struct item {
		uint32_t idx;
		std::string value;

		bool operator < (const item& other)
		{
			return idx < other.idx;
		}
	};
}

void U482::operator()()
{
	uint32_t n;
	std::cin >> n;
	std::string line;
	std::getline(std::cin, line);
	std::vector<item> src;
	while (n--) {
		src.clear();
		std::getline(std::cin, line);
		std::getline(std::cin, line);
		std::transform(std::istream_iterator<size_t>(std::stringstream(line)), std::istream_iterator<size_t>(), std::back_inserter(src), [&](size_t n) { item i; i.idx = n; return i; });
		std::getline(std::cin, line);
		auto src_pointer = src.begin();
		std::for_each(std::istream_iterator<std::string>(std::stringstream(line)), std::istream_iterator<std::string>(), [&](const std::string& val) { (src_pointer++)->value = val; });
		std::sort(src.begin(), src.end());
		std::for_each(src.begin(), src.end(), [&](item& itm) { std::cout << itm.value << std::endl; });
		std::cout << std::endl;
	}
}
