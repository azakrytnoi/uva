#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10131.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
	UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
	U10131 instance;
	instance();
}

namespace {
	typedef std::pair<int16_t, std::pair<int16_t, int16_t>> elephant;

	class solution {
		std::vector<elephant> elephants_;
		std::vector<int16_t> path_;
		int gmax_;
		int end_pos_;
	public:
		solution() : elephants_(), path_(), gmax_(1), end_pos_(0) {}

		friend std::istream& operator >> (std::istream& in, solution& sol);
		friend std::ostream& operator << (std::ostream& out, solution& sol);

		void solve();

	private:
		void print_path(int end, std::ostream& out);
	};

	std::istream& operator >> (std::istream& in, solution& sol)
	{
		std::pair<int16_t, int16_t> ws;
		while (std::cin >> ws.first >> ws.second) {
			sol.elephants_.push_back(std::make_pair(sol.elephants_.size() + 1, ws));
		}
		sol.elephants_.shrink_to_fit();
		return in;
	}

	std::ostream& operator << (std::ostream& out, solution& sol)
	{
		out << sol.gmax_ << std::endl;
		sol.print_path(sol.end_pos_, out);
		return out;
	}

	void solution::solve()
	{
		std::sort(elephants_.begin(), elephants_.end(), [](const elephant& e1, const elephant e2) {
			return e1.second.second > e2.second.second;
		});
		std::stable_sort(elephants_.begin(), elephants_.end(), [](const elephant& e1, const elephant e2) {
			return e1.second.first < e2.second.first;
		});
		std::vector<int16_t> s(elephants_.size());
		path_.resize(elephants_.size());

		s[0] = 1;
		path_[0] = -1;
		for (size_t i = 1; i < elephants_.size(); i++) {
			int16_t lprev = -1;
			s[i] = 1;
			for (size_t j = 0; j < i; j++) {
				if (elephants_[j].second.first < elephants_[i].second.first
					&& elephants_[j].second.second > elephants_[i].second.second
					&& s[i] < s[j] + 1) {
					s[i] = s[j] + 1;
					lprev = int16_t(j);
				}
			}
			path_[i] = lprev;
			if (gmax_ < s[i]) {
				gmax_ = s[i];
				end_pos_ = int(i);
			}
		}
	}

	void solution::print_path(int end, std::ostream& out)
	{
		if (end > -1) {
			print_path(path_[end], out);
			out << elephants_[end].first << std::endl;
		}
	}
}  // namespace

void U10131::operator()()
{
	solution sol;
	std::cin >> sol;
	sol.solve();
	std::cout << sol;
}
