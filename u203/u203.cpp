#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u203.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cmath>
#include <memory>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U203 instance;
    instance();
}

namespace
{

typedef long double real;

template <typename T>
class vector2D {
private:
    T x_;
    T y_;

public:
    vector2D(T x = 0, T y = 0) : x_(x), y_(y) {}

    T x() const {
        return x_;
    }
    T y() const {
        return y_;
    }
    void x(T v) {
        x_ = v;
    }
    void y(T v) {
        y_ = v;
    }

    T mod() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }
    T dot(vector2D<T>& rhs) const {
        return x_ * rhs.x_ + y_ * rhs.y_;
    }
    T angle(vector2D<T>& rhs) const {
        return std::acos(dot(rhs) / (mod() * rhs.mod()) );
    }
};

class ship {
private:
    vector2D<real> coords_;
    real velocity_;
    real curse_;
public:
    ship(real x, real y, real velocity, real curse) : coords_(x, y), velocity_(velocity), curse_(curse) {}

    real bearing(ship& rhs) const;
};

class solution
{
    std::string name_;
    ship our_;
    std::vector<std::shared_ptr<ship>> ships;
public:
    solution();

    friend std::istream& operator >>(std::istream& in, solution& sol);
    friend std::ostream& operator <<(std::ostream& out, const solution& sol);

    operator bool() const {
        return true;
    }
    solution& operator()();

private:
};

std::istream& operator >> (std::istream& in, solution& sol)
{
    std::getline(in, sol.name_);
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

void U203::operator()() const
{
    solution sol;
    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
