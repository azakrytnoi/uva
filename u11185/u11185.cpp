#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#endif

#include "u11185.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <string>

namespace {
template<size_t BASE>
class base_iterator : public std::iterator<std::input_iterator_tag, char> {
    int32_t number_;

public:
    explicit base_iterator(int32_t  number) : number_(number) {}

    char operator* ()
    {
        return (number_ % BASE) + '0';
    }

    base_iterator& operator++()
    {
        number_ /= BASE;
        return *this;
    }
    base_iterator operator++(int)
    {
        base_iterator tmp(*this);
        operator++();
        return tmp;
    }
    bool operator==(const base_iterator& rhs)
    {
        return number_==rhs.number_;
    }
    bool operator!=(const base_iterator& rhs)
    {
        return number_!=rhs.number_;
    }

};
}

void U11185::operator()()
{
    typedef base_iterator<3> base3_iterator;
    int32_t N;
    std::string translated;
    translated.reserve(20);
    base3_iterator bend(0);
    while (std::cin >> N && N >= 0) {
        if (N != 0) {
            base3_iterator bit(N);
            translated.clear();
            std::copy (bit, bend, std::back_inserter(translated));
            std::reverse(translated.begin(), translated.end());
            std::cout << translated << std::endl;
        } else {
            std::cout << "0" << std::endl;
        }
    }
}