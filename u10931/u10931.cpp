#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#endif

#include "u10931.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace {
class bit_iterator : public std::iterator<std::input_iterator_tag, char> {
    uint64_t number_;
    uint16_t& parity_;

public:
    explicit bit_iterator(uint64_t number, uint16_t& parity) : number_(number), parity_(parity) {}

    uint16_t parity() const
    {
        return parity_;
    }

    char operator* ()
    {
        return (number_ & 0x01) + '0';
    }

    bit_iterator& operator++()
    {
        parity_ += (number_ & 0x01);
        number_ >>= 1;;
        return *this;
    }
    bit_iterator operator++(int)
    {
        bit_iterator tmp(*this);
        operator++();
        return tmp;
    }
    bool operator==(const bit_iterator& rhs)
    {
        return number_==rhs.number_;
    }
    bool operator!=(const bit_iterator& rhs)
    {
        return number_!=rhs.number_;
    }
};
}

void U10931::operator()()
{
    std::string binary;
    binary.reserve(65);
    uint64_t number;
    uint16_t parity;
    bit_iterator b_end(0, parity);
    while (std::cin >> number && number != 0) {
        binary.clear();
        parity = 0;
        bit_iterator b_number (number, parity);
        std::copy (b_number, b_end, std::back_inserter(binary));
        std::reverse(binary.begin(), binary.end());
        std::cout << "The parity of " << binary << " is "
                  << parity << " (mod 2)." << std::endl;
    }
}
