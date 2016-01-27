#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <iosfwd>

template<typename Tp>
class evaluator
{
public:
	evaluator(const std::string& source);
	~evaluator();

private:
	std::streambuf* original_;
};

template<typename Tp>
evaluator<Tp>::evaluator(const std::string& source)
{
	std::ifstream in(source.c_str());
	original_ = std::cin.rdbuf(in.rdbuf());
	Tp tp;
	std::cout << tp() << std::endl;
}

template<typename Tp>
evaluator<Tp>::~evaluator()
{
	std::cin.rdbuf(original_);
}