#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u619.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cctype>
#include <sstream>

#include "bigint.h"

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U619 instance;
	instance();
}

namespace {
	class checker {
	public:
		static std::string translate(const std::string& source);

	private:
		static std::string translate_number(const std::string& source);
		static std::string translate_string(const std::string& source);
	};

	std::string checker::translate(const std::string & source)
	{
		if (std::isdigit(source[0])) {
			return translate_number(source);
		}
		return translate_string(source);
	}

	std::string checker::translate_number(const std::string & source)
	{
		math::BigInteger num = source;
		return source;
	}

	std::string checker::translate_string(const std::string & source)
	{
		math::BigInteger bi = "1";
		math::BigInteger result;
		std::for_each(source.rbegin(), source.rend(), [&](auto ch) {
			std::stringstream ss;
			ss << ((ch - 'a' + 1) % 26);
			math::BigInteger letter = ss.str();
			letter *= bi;
			result += letter;
			bi *= 26;
		});
		std::stringstream ss;
		ss << std::setw(22) << source << result;
		return ss.str();
	}
}

void U619::operator()()
{
	std::string line;
	while (std::getline(std::cin, line) && line != "*")
	{
		std::cout << checker::translate(line) << std::endl;
	}
}