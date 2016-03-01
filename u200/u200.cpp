#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u200.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

U200::U200()
{
}


U200::~U200()
{
}

extern "C" { 	UVA_API_EXPORT void __cdecl invoke(); } void __cdecl invoke() { 	U200 instance; 	instance(); }
void U200::operator()()
{
    while (std::cin) {
        std::vector<std::string> index;
        std::string ordering;
        {
            std::string line;
            while (std::getline(std::cin, line)) {
                char ch = line[0];
                if (ch != '#') {
                    index.push_back(line);
                    if (ordering.find(ch) == std::string::npos) {
                        ordering.push_back(ch);
                    }
                } else {
                    break;
                }
            }
        }
        {
            std::for_each (ordering.begin(), ordering.end(), [&](char start_ch) {
                std::vector<std::string> selected;
                std::copy_if(index.begin(), index.end(), std::back_inserter(selected), [&](std::string & line) {
                    return line[0] == start_ch;
                });
                if (selected.size() > 1) {
                    size_t idx = 1;
                    while (!selected.empty()) {
                        std::vector<std::string> working;
                        std::copy_if(selected.begin(), selected.end(), std::back_inserter(working), [&](std::string & line) {
                            return line.size() > idx;
                        });
                        for (size_t i = 1; i < working.size(); ++i) {
                            char prev_ch = working[i - 1][idx];
                            char curr_ch = working[i][idx];
                            if (prev_ch == curr_ch) continue;
                            size_t prev_pos, curr_pos;
                            if ((curr_pos = ordering.find(curr_ch)) == std::string::npos) {
                                const size_t prev_pos = ordering.find(prev_ch);
                                if (prev_pos != std::string::npos) {
                                    ordering.insert(prev_pos + 1, 1, curr_ch);
                                }
                            } else if ((working[i - 1][idx - 1] == working[i][idx - 1]) && (prev_pos = ordering.find(prev_ch)) > curr_pos) {
                                std::swap(ordering[prev_pos], ordering[curr_pos]);
                            }
                        }
                        idx++;
                        selected = working;
                    }
                }
            });
        }
        std::cout << ordering << std::endl;
    }
}
