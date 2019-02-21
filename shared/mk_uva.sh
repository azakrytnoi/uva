#!/bin/bash

if [ "$#" -ne 2 ]
then
	echo "must be 2 parameters: ./mk_uva <uva_no> <vol_no>"
	exit
fi

mkdir -p ../vol$2/u$1

if [ ! -f ../vol$2/Makefile ]
then
	cp ../vol001/Makefile ../vol$2
fi

cat > "../vol$2/u$1/u$1.h" <<EOF
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

cat > "../vol$2/u$1/u$1.cpp" <<EOF
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

void U$1::operator()() const
{
    solution_t sol;
    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
EOF

cat >>uvas.h <<EOF
#include "../u$1/u$1.h"
EOF

touch "../vol$2/u$1/u$1.txt"

sed "s/u100/u$1/g" ../vol001/u100/Makefile > ../vol$2/u$1/Makefile
sed "s/u100/u$1/g" ../vol001/u100/u100.vcxproj > ../vol$2/u$1/u$1.vcxproj
sed "s/u100/u$1/g" ../vol001/u100/u100.vcxproj.filters > ../vol$2/u$1/u$1.vcxproj.filters

make 

cd "../vol$2/u$1"
make depend
cd ../../shared


git add ../*.h ../*.cpp ../*.txt ../*.vcxproj* ../*/Makefile ../*/*/Makefile
git commit -m "initial"
