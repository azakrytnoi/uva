#define UVA_API_EXPORT __declspec(dllexport)

#include "u478.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <cmath>
#include <sstream>

namespace {

typedef std::pair<float_t, float_t> point;

std::istream& operator >> (std::istream& in, point& p)
{
    in >> p.first >> p.second;
    return in;
}

float_t distance (const point& p1, const point& p2)
{
    return std::sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
}

class shape {
public:
    enum class kind : char {
        RECTANGLE = 'r', CIRCLE = 'c', TRIANGLE ='t'
    };
    const kind kind_;
    const size_t seq_;

    shape(kind k, size_t seq) : kind_(k), seq_(seq) {}
    virtual ~shape() {}

    virtual bool point_inside (const point& p) = 0;

    size_t seq () const
    {
        return seq_;
    }
};

class rectangle : public shape {
    point topLeft_;
    point bottomRight_;

public:
    rectangle(size_t seq, point& topLeft, point& bottomRight) : shape(kind::RECTANGLE, seq), topLeft_(topLeft), bottomRight_(bottomRight) {}
    virtual ~rectangle() {}

    virtual bool point_inside(const point& p)
    {
        return topLeft_.first < p.first && p.first < bottomRight_.first
               && topLeft_.second > p.second && p.second > bottomRight_.second;
    }

};

class circle : public shape {
    point center_;
    float_t radius_;

public:
    circle(size_t seq, point center, float_t radius) : shape(kind::CIRCLE, seq), center_(center), radius_(radius) {}
    virtual ~circle() {}

    virtual bool point_inside(const point& p)
    {
        return distance (center_, p) < radius_;
    }

};

class triange : public shape {
    point a_;
    point b_;
    point c_;

public:
    triange(size_t seq, point& a, point& b, point& c) : shape(kind::TRIANGLE, seq), a_(a), b_(b), c_(c) {}
    virtual ~triange() {}

    virtual bool point_inside (const point& p)
    {
        float alpha = ((b_.second - c_.second)*(p.first - c_.first) + (c_.first - b_.first)*(p.second - c_.second)) /
                      ((b_.second - c_.second)*(a_.first - c_.first) + (c_.first - b_.first)*(a_.second - c_.second));
        float beta = ((c_.second - a_.second)*(p.first - c_.first) + (a_.first - c_.first)*(p.second - c_.second)) /
                     ((b_.second - c_.second)*(a_.first - c_.first) + (c_.first - b_.first)*(a_.second - c_.second));
        float gamma = 1.0f - alpha - beta;
        return alpha > 0 && beta > 0 && gamma > 0;
    }
};
}

void U478::operator()()
{
    std::vector<std::shared_ptr<shape>> shapes;
    shapes.reserve(10);
    std::string line;
    while (std::getline(std::cin, line) && line[0] != '*') {
        std::stringstream iss(line.substr(2));
        switch (line[0]) {
        case 'r' : {
            point top, bottom;
            iss >> top >> bottom;
            auto rect = std::make_shared<rectangle>(shapes.size() + 1, top, bottom);
            shapes.push_back(rect);
        }
        break;
        case 'c' : {
            point center;
            float radius;
            iss >> center >> radius;
            auto circ = std::make_shared<circle>(shapes.size() + 1, center, radius);
            shapes.push_back(circ);
        }
        break;
        case 't' : {
            point a, b, c;
            iss >> a >> b >> c;
            auto trn = std::make_shared<triange>(shapes.size() + 1, a, b, c);
            shapes.push_back(trn);
        }
        break;
        }
    }
    point p;
    point pend(9999.9f, 9999.9f);
    size_t pseq (0);
    while (std::cin >> p && p != pend) {
        pseq++;
        bool found(false);
        std::for_each (shapes.begin(), shapes.end(), [&](auto sh) {
            if (sh->point_inside(p)) {
                found = true;
                std::cout << "Point " << pseq << " is contained in figure " << sh->seq() << std::endl;
            }
        });
        if (!found) {
            std::cout << "Point " << pseq << " is not contained in any figure" << std::endl;
        }
    }
}
