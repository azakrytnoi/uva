#!/bin/bash

mkdir ../u$1

cat > "../u$1/u$1.h" <<EOF
#pragma once

class U$1
{
public:
	static const char * libname()
	{
		return "u$1";
	}
    U$1() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U$1> pu$1;
#endif
EOF

cat > "../u$1/u$1.cpp" <<EOF
#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u$1.h"

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
	U$1 instance;
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

void U$1::operator()() const
{
    solution sol;
    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
EOF

cat >>uvas.h <<EOF
#include "../u$1/u$1.h"
EOF

touch "../u$1/u$1.txt"

sed "s/u100/u$1/g" ../u100/Makefile > ../u$1/Makefile
sed "s/u100/u$1/g" ../u100/u100.vcxproj > ../u$1/u$1.vcxproj
sed "s/u100/u$1/g" ../u100/u100.vcxproj.filters > ../u$1/u$1.vcxproj.filters

make 

cd "../u$1"
make depend
cd ../shared


git add ../*.h ../*.cpp ../*.txt ../*.vcxproj* ../*/Makefile
