#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10008.h"

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
    U10008 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : source_(), freq_() {}

        operator bool()
        {
            return not source_.empty();
        }

        solution_t& operator()()
        {
            freq_.clear();
            std::map<char, size_t> counter;

            for (auto ch : source_) {
                if (std::isalpha(ch)) {
                    counter[std::toupper(ch)]++;
                }
            }

            freq_.assign(counter.begin(), counter.end());
            std::sort(freq_.begin(), freq_.end(), [](const std::pair<char, size_t>& lhs, const std::pair<char, size_t>& rhs) {
                if (lhs.second != rhs.second) {
                    return lhs.second > rhs.second;
                }

                return lhs.first < rhs.first;
            });
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            sol.source_.clear();

            if (in) {
                size_t n(0);
                in >> n;
                std::string dummy;
                std::getline(in, dummy);

                while (n--) {
                    std::getline(in, dummy);
                    sol.source_ += dummy;
                }
            }

            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            for (auto& pair : sol.freq_) {
                out << pair.first << ' ' << pair.second << std::endl;
            }

            return out;
        }

    private:
        std::string source_;
        std::vector<std::pair<char, size_t>> freq_;
    };

}

void U10008::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
