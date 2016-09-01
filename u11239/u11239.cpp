#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u11239.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <set>
#include <iterator>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11239 instance;
    instance();
}
void U11239::operator()() const
{
    std::string line;
    std::map<std::string, std::string> students;
    std::map<std::string, std::set<std::string>> projects;
    std::string project;
    while (std::getline(std::cin, line) && line[0] != '0') {
        projects.clear();
        students.clear();
        do {
            if (std::isupper(line[0])) {
                project = line;
                projects[project].clear();
            } else {
                if (students.find(line) == students.end()) {
                    students[line] = project;
                    projects[project].insert(line);
                } else {
                    if (students[line] != project) {
                        std::for_each(projects.begin(), projects.end(), [&](auto& enroled) {
                            enroled.second.erase(line);
                        });
                    }
                }
            }
        } while (std::getline(std::cin, line) && line[0] != '1');
        std::vector<std::pair<std::string, size_t>> result;
        result.reserve(projects.size());
        std::transform(projects.begin(), projects.end(), std::back_inserter(result), [](auto item) -> std::pair<std::string, size_t> {
            return std::make_pair(item.first, item.second.size());
        });
        std::sort(result.begin(), result.end(), [](auto i1, auto i2) -> bool {
            if (i1.second == i2.second)
            {
                return i1.first < i2.first;
            }
            return i1.second > i2.second;
        });
        std::for_each(result.begin(), result.end(), [](const std::pair<std::string, size_t>& item) {
            std::cout << item.first << " " << item.second << std::endl;
        });
        std::cout << std::endl;
    }
}
