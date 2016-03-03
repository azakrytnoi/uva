#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u403.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <regex>

namespace {
	class driver {
		char page_[60][60];
		std::regex parser_;
		std::smatch match_;
		//    std::string c1_font_;
		std::map<char, std::vector<std::string>> c5_font_;
	public:
		driver() :
			parser_("\\.([PLRC])[^C]*C([15]) *(\\d+ )(\\d+ )?[^\\|]*\\|([^\\|]*)\\|.*")
			, match_(), /*c1_font_("ABCDEFGHIJKLMNOPQRSTUVWXYZ "), */c5_font_()
		{
			c5_font_['A'] = { ".***..", "*...*.", "*****.", "*...*.", "*...*." };
			c5_font_['B'] = { "****..", "*...*.", "****..", "*...*.", "****.." };
			c5_font_['C'] = { ".****.", "*...*.", "*.....", "*.....", ".****." };
			c5_font_['D'] = { "****..", "*...*.", "*...*.", "*...*.", "****.." };
			c5_font_['E'] = { "*****.", "*.....", "***...", "*.....", "*****." };
			c5_font_['F'] = { "*****.", "*.....", "***...", "*.....", "*....." };
			c5_font_['G'] = { ".****.", "*.....", "*..**.", "*...*.", ".***.." };
			c5_font_['H'] = { "*...*.", "*...*.", "*****.", "*...*.", "*...*." };
			c5_font_['I'] = { "*****.", "..*...", "..*...", "..*...", "*****." };
			c5_font_['J'] = { "..***.", "...*..", "...*..", "*..*..", ".**..." };
			c5_font_['K'] = { "*...*.", "*..*..", "***...", "*..*..", "*...*." };
			c5_font_['L'] = { "*.....", "*.....", "*.....", "*.....", "*****." };
			c5_font_['M'] = { "*...*.", "**.**.", "*.*.*.", "*...*.", "*...*." };
			c5_font_['N'] = { "*...*.", "**..*.", "*.*.*.", "*..**.", "*...*." };
			c5_font_['O'] = { ".***..", "*...*.", "*...*.", "*...*.", ".***.." };
			c5_font_['P'] = { "****..", "*...*.", "****..", "*.....", "*....." };
			c5_font_['Q'] = { ".***..", "*...*.", "*...*.", "*..**.", ".****." };
			c5_font_['R'] = { "****..", "*...*.", "****..", "*..*..", "*...*." };
			c5_font_['S'] = { ".****.", "*.....", ".***..", "....*.", "****.." };
			c5_font_['T'] = { "*****.", "*.*.*.", "..*...", "..*...", ".***.." };
			c5_font_['U'] = { "*...*.", "*...*.", "*...*.", "*...*.", ".***.." };
			c5_font_['V'] = { "*...*.", "*...*.", ".*.*..", ".*.*..", "..*..." };
			c5_font_['W'] = { "*...*.", "*...*.", "*.*.*.", "**.**.", "*...*." };
			c5_font_['X'] = { "*...*.", ".*.*..", "..*...", ".*.*..", "*...*." };
			c5_font_['Y'] = { "*...*.", ".*.*..", "..*...", "..*...", "..*..." };
			c5_font_['Z'] = { "*****.", "...*..", "..*...", ".*....", "*****." };
			c5_font_[' '] = { "......", "......", "......", "......", "......" };
			clean_page();
		}

		~driver()
		{
		}

		friend std::istream& operator >> (std::istream& in, driver& d)
		{
			std::string cmmand, line;
			if (std::getline(in, line) && !line.empty()) {
				if (line[1] != 'E' && std::regex_match(line, d.match_, d.parser_)) {
					uint32_t font = std::atoi(d.match_[2].str().c_str());
					uint32_t row = std::atoi(d.match_[3].str().c_str()) - 1;
					switch (d.match_[1].str()[0]) {
					case 'P': {
						uint32_t col = std::atoi(d.match_[4].str().c_str()) - 1;
						d.place(font, row, col, d.match_[5]);
					}
							  break;
					case 'L': {
						d.left(font, row, d.match_[5]);
					}
							  break;
					case 'R': {
						d.right(font, row, d.match_[5]);
					}
							  break;
					case 'C': {
						d.center(font, row, d.match_[5]);
					}
							  break;
					}
				}
				else {
					if (line == ".EOP") {
						d.flush_page();
						d.clean_page();
					}
				}
			}
			return in;
		}

	private:
		void clean_page();
		void place(uint32_t font, uint32_t row, uint32_t col, const std::string& text);
		void right(uint32_t font, uint32_t row, const std::string& text);
		void left(uint32_t font, uint32_t row, const std::string& text);
		void center(uint32_t font, uint32_t row, const std::string& text);
		void flush_page();
		void print_c1_at(uint32_t row, uint32_t col, const std::string& text);
		void print_c5_at(uint32_t row, uint32_t col, const std::string& text);
		std::pair<uint32_t, std::string> center_text(const std::string& text, uint32_t width);
		std::pair<uint32_t, std::string> right_text(const std::string& text, uint32_t width);
		std::pair<uint32_t, std::string> left_text(const std::string& text, uint32_t width);
	};

	void driver::clean_page()
	{
		std::for_each(page_, page_ + 60, [](char * row) {
			std::generate_n(row, 60, []() {
				return '.';
			});
		});
	}

	void driver::place(uint32_t font, uint32_t row, uint32_t col, const std::string& text)
	{
		switch (font) {
		case 1: {
			std::pair<uint32_t, std::string> aligned = left_text(text, 61 - col);
			if (col + aligned.second.length() > 60) {
				aligned.second.erase(aligned.second.end() - 1);
			}
			print_c1_at(row, col, aligned.second);
		}
				break;
		case 5: {
			std::pair<uint32_t, std::string> aligned = left_text(text, (66 - col) / 6);
			print_c5_at(row, col, aligned.second);
		}
				break;
		}
	}

	void driver::right(uint32_t font, uint32_t row, const std::string& text)
	{
		switch (font) {
		case 1: {
			std::pair<uint32_t, std::string> aligned = right_text(text, 60);
			print_c1_at(row, aligned.first, aligned.second);
		}
				break;
		case 5: {
			std::pair<uint32_t, std::string> aligned = right_text(text, 10);
			print_c5_at(row, aligned.first * 6, aligned.second);
		}
				break;
		}
	}

	void driver::left(uint32_t font, uint32_t row, const std::string& text)
	{
		switch (font) {
		case 1: {
			std::pair<uint32_t, std::string> aligned = left_text(text, 60);
			print_c1_at(row, aligned.first, aligned.second);
		}
				break;
		case 5: {
			std::pair<uint32_t, std::string> aligned = left_text(text, 10);
			print_c5_at(row, aligned.first * 6, aligned.second);
		}
				break;
		}
	}

	void driver::center(uint32_t font, uint32_t row, const std::string& text)
	{
		switch (font) {
		case 1: {
			std::pair<uint32_t, std::string> aligned = center_text(text, 60);
			print_c1_at(row, (aligned.second.length() & 0x01) == 0 ? aligned.first : aligned.first + 1, aligned.second);
		}
				break;
		case 5: {
			std::pair<uint32_t, std::string> aligned = center_text(text, 10);
			print_c5_at(row, (aligned.second.length() & 0x01) == 0 ? aligned.first * 6 : aligned.first * 6 + 3, aligned.second);
		}
				break;
		}
	}

	std::pair<uint32_t, std::string> driver::right_text(const std::string& text, uint32_t width)
	{
		std::string output(text);
		while (output.length() > width) {
			output = output.substr(1, output.length() - 1);
		}
		uint32_t col(uint32_t(width - output.length()));
		return std::make_pair(col, output);
	}

	std::pair<uint32_t, std::string> driver::left_text(const std::string& text, uint32_t width)
	{
		std::string output(text);
		while (output.length() > width) {
			output = output.substr(0, output.length() - 2);
		}
		return std::make_pair(0, output);
	}

	std::pair<uint32_t, std::string> driver::center_text(const std::string& text, uint32_t width)
	{
		std::string output(text);
		while (output.length() > width) {
			output = output.substr(1, output.length() - 2);
		}
		uint32_t col(uint32_t((width - output.length()) / 2));
		return std::make_pair(col, output);
	}

	void driver::print_c1_at(uint32_t row, uint32_t col, const std::string& text)
	{
		int idx = 0;
		std::transform(text.begin(), text.end(), page_[row] + col, [&](const char ch) {
			idx++;
			return ch == ' ' ? page_[row][col + idx - 1] : ch;
		});
	}

	void driver::print_c5_at(uint32_t row, uint32_t col, const std::string& text)
	{
		for (uint32_t i = 0; i < 5 && (row + i) < 60; i++) {
			uint32_t current_col(col);
			std::for_each(text.begin(), text.end(), [&](const char ch) {
				int idx = 0;
				std::string line(c5_font_[ch][i]);
				if (current_col > 54) {
					line = line.substr(0, 60 - current_col);
				}
				std::transform(line.begin(), line.end(), page_[row + i] + current_col, [&](const char ch) {
					idx++;
					return ch == '.' ? page_[row + i][current_col + idx - 1] : ch;
				});
				current_col += 6;
			});
		}
	}

	void driver::flush_page()
	{
		std::for_each(page_, page_ + 60, [](const char row[60]) {
			std::for_each(row, row + 60, [](const char ch) {
				std::cout << ch;
			});
			std::cout << std::endl;
		});
		static std::string delimiter(60, '-');
		std::cout << std::endl << delimiter << std::endl << std::endl;
	}
}

U403::U403() {}

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U403 instance;
	instance();
}
void U403::operator()()
{
	driver d;
	while (std::cin >> d);
}