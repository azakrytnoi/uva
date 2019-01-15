#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u118.h"

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
	U118 instance;
	instance();
}

namespace
{

class solution
{
public:
    solution() { }

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const { return true; }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
  return in;
}

std::ostream& operator << (std::ostream& out, const solution& sol)
{
  return out;
}

solution& solution::operator()()
{
  return *this;
}

}

void U118::operator()() const
{
    solution sol;
    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
