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
void __cdecl invoke() {
	U619 instance;
	instance();
}

namespace {
	class checker {
	public:
		static std::string translate(std::string& source);

	private:
		static std::string change_base(std::string& source, int from, int to, int base);
		static void process_carry(std::vector<int>& data, int to);

		template<typename T>
		static void print(std::ostream& out, T& data);
	};

	template<typename T>
	void checker::print(std::ostream& out, T& data) {
		uint32_t pos(0);
		std::reverse(data.begin(), data.end());
		std::stringstream temp;
		std::for_each(data.begin(), data.end(), [&](auto digit) {
			pos = ((++pos) % 3);
			temp << digit;
			if (pos == 0) {
				temp << ',';
			}
		});
		std::string result(temp.str());
		if (result.back() == ',') {
			result.erase(result.size() - 1);
		}
		std::reverse(result.begin(), result.end());
		out << result;
	}

	std::string checker::translate(std::string & source) {
		if (std::isdigit(source[0])) {
			return change_base(source, 10, 26, '0');
		}
		return change_base(source, 26, 10, 'a' - 1);
	}

	std::string checker::change_base(std::string& source, int from, int to, int base) {
		std::vector<int> internal;
		internal.push_back(0);
		std::for_each(source.begin(), source.end(), [&](auto ch) {
			std::transform(internal.begin(), internal.end(), internal.begin(), [&](auto digit) {return digit * from; });
			internal.back() += ch - base;
			process_carry(internal, to);
		});
		std::stringstream out;
		if (from == 10) {
			std::string translated;
			translated.resize(internal.size());
			std::transform(internal.begin(), internal.end(), translated.begin(), [](auto ch) {return ch + 'a' - 1; });
			out.setf(std::ios::left);
			out << std::setw(22) << translated;
			print(out, source);
		}
		else {
			out.setf(std::ios::left);
			out << std::setw(22) << source;
			print(out, internal);
		}
		return out.str();
	}

	void checker::process_carry(std::vector<int>& data, int base_to) {
		for (auto it = data.rbegin(); it != data.rend() - 1; ++it) {
			*(it + 1) += *it / base_to;
			*it %= base_to;
		}
		int tmp = data[0];
		while (tmp >= base_to) {
			data.insert(data.begin(), tmp / base_to);
			data[1] = tmp % base_to;
			tmp /= base_to;
		}
	}
}

void U619::operator()() {
	std::string line;
	while (std::getline(std::cin, line) && line != "*") {
		std::cout << checker::translate(line) << std::endl;
	}
}
