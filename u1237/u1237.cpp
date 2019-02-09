#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u1237.h"

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
	U1237 instance;
	instance();
}

namespace
{

class solution_t
{
public:
    solution_t() { }

    friend std::istream& operator >>(std::istream& in, solution_t& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

    operator bool() const { return true; }
    solution_t& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution_t& sol)
{
  return in;
}

std::ostream& operator << (std::ostream& out, const solution_t& sol)
{
  return out;
}

solution_t& solution_t::operator()()
{
  return *this;
}

}

void U1237::operator()() const
{
    solution_t sol;
    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
