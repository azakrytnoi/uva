#pragma once

#include <algorithm>

namespace math
{

template<typename T>
T gcd (T a, T b)
{
    if (a < b) {
        std::swap (a, b);
    }
    T t = a % b;
    if (t == 0) {
        return b;
    }
    return gcd (b, t);
}

template<typename T>
T lcm(T a, T b)
{
	return (a * b) / gcd(a, b);
}

}
