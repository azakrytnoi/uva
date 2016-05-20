#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u119.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U119 instance;
    instance();
}

namespace
{

class group
{
    std::vector<std::pair<std::string, int32_t>> names_;
public:
    group() : names_() {}

    friend std::istream& operator >> (std::istream& in, group &gr);
    friend std::ostream& operator << (std::ostream& out, const group& gr);

private:
};

std::istream& operator >> (std::istream& in, group &gr)
{
    int N;
    if (in >> N) {
        gr.names_.clear();
        std::string name;
        gr.names_.reserve(N);
        std::vector<std::string> names;
        names.reserve(N);
        std::map<std::string, int32_t> temp;
        std::generate_n(std::back_inserter(names), N, [&]() {
            in >> name;
            return name;
        });
        while (N--) {
            in >> name;
            int32_t gift, n;
            in >> gift >> n;
            temp[name] -= gift;
            if (n > 0) {
                temp[name] += gift % n;
                gift /= n;
                while (n--) {
                    in >> name;
                    temp[name] += gift;
                }
            }
        }
        std::transform(names.begin(), names.end(), std::back_inserter(gr.names_), [&](const std::string& res) {
            return std::make_pair(res, temp[res]);
        });
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const group& gr)
{
    std::for_each(gr.names_.begin(), gr.names_.end(), [&](const std::pair<std::string, int32_t>& res) {
        out << res.first << " " << res.second << std::endl;
    });
    return out;
}

}  // namespace

void U119::operator()() const
{
    group gr;
    while (std::cin >> gr) {
        std::cout << gr << std::endl;
    }
}
