#!/bin/bash

if [ -z "$1" ]
then
   echo "No number"
   exit 1
fi

cat > "u$1.h" <<EOF
#pragma once

class U$1
{
public:
    U$1() {}

    void operator()();
};
EOF

cat > "u$1.cpp" <<EOF
#include "u$1.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

void U$1::operator()()
{

}
EOF

cat >>uvas.h <<EOF
#include "u$1.h"
EOF

touch "u$1.txt"

make depend
rm Makefile.bak

git add Makefile *.h *.cpp *.txt
