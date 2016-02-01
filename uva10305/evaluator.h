#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <iosfwd>
#include <sstream>

template<typename Tp>
class evaluator
{
public:
	evaluator(const std::string& source);
	~evaluator();

private:
};

namespace {
	template <typename T>
	class io_wrapper
	{
	public:
		explicit io_wrapper(T& stream, std::streambuf* streambuf) : stream_(stream)
		{
			streambuf_= stream_.rdbuf(streambuf);
		}

		~io_wrapper()
		{
			stream_.rdbuf(streambuf_);
		}

	private:
		T& stream_;
		std::streambuf* streambuf_;
	};

}

template<typename Tp>
evaluator<Tp>::evaluator(const std::string& source)
{
	std::ifstream in(source.c_str());
	std::stringstream out;
	Tp tp;
	{
		io_wrapper<std::ostream> wrap_in (std::cout, out.rdbuf());
		io_wrapper<std::istream> wrap_out (std::cin, in.rdbuf());
		tp();
	}
	std::cout << out.str();
}

template<typename Tp>
evaluator<Tp>::~evaluator()
{
}