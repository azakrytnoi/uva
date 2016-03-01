#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define UVA_API_EXPORT
#endif

#include "u10301.h"

#include <cstdint>
#include <limits>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

U10301::U10301()
{
}

namespace
{
class ring
{
public:
    ring() : mX(0.0), mY(0.0), mR(0.0) {}

    friend std::istream& operator >> (std::istream& in, ring& r)
    {
        in >> r.mX >> r.mY >> r.mR;
        return in;
    }

    bool intersect (const ring& other) const
    {
        long double distance = sqrt((mX - other.mX) * (mX - other.mX) + (mY - other.mY) * (mY - other.mY));
        if(distance < (mR + other.mR)) {
            if(distance + std::min(mR, other.mR) <= std::max(mR, other.mR)) {
                return false;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }


private:
    long double mX, mY, mR;
};
}

extern "C" { 	UVA_API_EXPORT void __cdecl invoke(); } void __cdecl invoke() { 	U10301 instance; 	instance(); }
void U10301::operator()()
{
    int16_t n_rings;
    std::vector<ring> rings;
    while ((std::cin >> n_rings) && n_rings > 0) {
        rings.clear();
        rings.reserve(n_rings);
        std::generate_n(std::back_inserter(rings), n_rings, []() -> ring {
            ring r;
            std::cin >> r;
            return r;
        });
        std::vector<std::vector<bool>> connected(rings.size());
        for(size_t i = 0; i < rings.size(); i++) {
            connected[i].resize(rings.size());
            connected[i][i] = true;
        }
        for (size_t i = 0; i < rings.size(); i++) {
            for (size_t j = 0; j < i; j++) {
                if (rings[i].intersect(rings[j])) {
                    connected[i][j] = connected[j][i] = true;
                }
            }
        }
        size_t max_glued (std::numeric_limits<size_t>::min());
        std::for_each(connected.begin(), connected.end(), [&](std::vector<bool>& row) {
            size_t glued = std::count_if(row.begin(), row.end(), [&](bool c) {
                return c;
            });
            max_glued = std::max(glued, max_glued);
        });
        std::cout << "The largest component contains " << max_glued << " rings." << std::endl;
    }
}
