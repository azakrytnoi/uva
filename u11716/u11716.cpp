#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11716.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U11716 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : cipher_(), plain_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        std::string cipher_;
        std::string plain_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::getline(in, sol.cipher_);
        sol.plain_.erase();
        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.plain_;
        return out;
    }

    solution& solution::operator()()
    {
        plain_.reserve(cipher_.length());
        size_t dim = std::sqrt(cipher_.length());

        if (dim * dim != cipher_.length()) {
            plain_ = "INVALID";
        } else {
            std::vector<std::string> matrix;
            matrix.resize(dim);
            auto ptr = cipher_.begin();

            for (auto& row : matrix) {
                row.assign(ptr, ptr + dim);
                ptr += dim;
            }

            for (size_t idx = 0; idx < dim; idx++) {
                for (auto& row : matrix) {
                    plain_ += row[idx];
                }
            }
        }

        return *this;
    }

}

void U11716::operator()() const
{
    size_t N;
    std::cin >> N;
    std::string dummy;
    std::getline(std::cin, dummy);
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
