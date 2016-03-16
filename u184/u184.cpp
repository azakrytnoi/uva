#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u184.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <list>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke() {
    U184 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution() :
                points_(), lines_() {
            points_.reserve(300);
        }

        friend std::istream& operator >>(std::istream& in, solution& sol) {
            sol.points_.clear();
            while (in) {
                std::pair<uint16_t, uint16_t> point;
                in >> point.first >> point.second;
                if (point.first == 0 && point.second == 0)
                    break;
                sol.points_.push_back(point);
            }
            std::sort(sol.points_.begin(), sol.points_.end(), [](auto p1, auto p2) {
                if (p1.first == p2.first) {
                    return p1.second < p2.second;
                }
                return p1.first < p2.first;
            });
            return in;
        }

        friend std::ostream& operator <<(std::ostream& out, const solution& sol) {
            std::for_each(sol.lines_.begin(), sol.lines_.end(), [&](auto line) {
                std::for_each(line.begin(), line.end(), [&](auto point) {
                            out << "(" << std::setw(4) << point.first << "," << std::setw(4) << point.second << ")";
                        });
                out << std::endl;
            });
            return out;
        }

        bool solve();
        bool empty() const {
            return points_.empty();
        }

    private:
        std::vector<std::pair<uint16_t, uint16_t>> points_;
        std::vector<std::vector<std::pair<uint16_t, uint16_t>>>lines_;

        static
        bool colinear(std::pair<uint16_t, uint16_t> &a, std::pair<uint16_t, uint16_t>& b, std::pair<uint16_t, uint16_t>& c) {
            return a.first * (b.second - c.second) + b.first * (c.second - a.second) + c.first * (a.second - b.second) == 0;
        }

        static
        bool already_marked(std::pair<uint16_t, uint16_t>& a, std::pair<uint16_t, uint16_t>& b, std::vector<std::pair<std::pair<uint16_t, uint16_t>,std::pair<uint16_t, uint16_t>>>& proceesed) {
            for (auto pp = proceesed.begin(); pp != proceesed.end(); ++pp) {
                bool r1 (colinear(a, b, pp->first));
                bool r2 (colinear(a, b, pp->second));
                if ( r1 && r2 ) {
                    return true;
                }
            }
            return false;

        }
    };

    bool solution::solve() {
        lines_.clear();
        std::vector<std::pair<std::pair<uint16_t, uint16_t>, std::pair<uint16_t, uint16_t>>>processed;
        for (auto pa = points_.begin(); pa != points_.end() - 2; ++pa) {
            for (auto pb = pa + 1; pb != points_.end() - 1; ++pb) {
                if (!already_marked(*pa, *pb, processed)) {
                    std::vector<std::pair<uint16_t, uint16_t>> line;
                    for (auto pc = pb + 1; pc != points_.end(); ++pc) {
                        if (colinear(*pa, *pb, *pc)) {
                            if (line.empty()) {
                                line.push_back(*pa);
                                line.push_back(*pb);
                                processed.push_back(std::make_pair(*pa, *pb));
                            }
                            line.push_back(*pc);
                        }
                    }
                    if (!line.empty()) {
                        lines_.push_back(line);
                    }
                }
            }
        }
        return !lines_.empty();
    }
}

void U184::operator()() {
    solution sol;
    while (std::cin >> sol && !sol.empty()) {
        if (sol.solve()) {
            std::cout << "The following lines were found:" << std::endl;
            std::cout << sol;
        } else {
            std::cout << "No lines were found" << std::endl;
        }
    }
}
