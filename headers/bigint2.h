/*
 * bigint2.h
 *
 *  Created on: Jan 25, 2019
 *      Author: aleksey
 */

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace math {

class uint_big_t
{
public:
    uint_big_t(uint64_t n = 0) : number_()
    {
        while (n > 0)
        {
            number_.push_back((n % 10));
            n /= 10;
        }

        if (number_.empty())
        {
            number_.push_back(0);
        }
    }

    uint_big_t(const uint_big_t& rhs) : number_(rhs.number_.begin(), rhs.number_.end())
    {}
    uint_big_t(uint_big_t&& rhs) : number_(std::move(rhs.number_))
    {}

    uint_big_t& operator = (const uint_big_t& rhs)
    {
    	number_.assign(rhs.number_.begin(), rhs.number_.end());
    	return *this;
    }
    uint_big_t& operator = (uint_big_t&& rhs)
    {
        number_ = std::move(rhs.number_);
        return *this;
    }

    bool operator < (const uint_big_t& rhs) const;

    bool operator == (const uint_big_t& rhs) const;

    bool operator <= (const uint_big_t& rhs) const
    {
        return *this < rhs || *this == rhs;
    }

    bool operator != (const uint_big_t& rhs) const
	{
    	return not (*this == rhs);
	}

    uint_big_t& operator+=(const uint_big_t& rhs);

    uint_big_t& operator++()
    {
        *this += 1;
        return *this;
    }

    uint_big_t operator++(int /*b*/)
    {
        uint_big_t tmp(*this);
        *this += 1;
        return tmp;
    }

    friend uint_big_t operator + (const uint_big_t& lhs, const uint_big_t& rhs)
    {
        uint_big_t tmp(lhs);
        tmp += rhs;
        return tmp;
    }

    uint_big_t& operator -= (const uint_big_t& rhs);

    uint_big_t& operator --()
    {
        *this -= 1;
        return *this;
    }

    uint_big_t operator --(int /*b*/)
    {
        uint_big_t tmp(*this);
        *this -= 1;
        return tmp;
    }

    friend uint_big_t operator-(const uint_big_t& lhs, uint_big_t& rhs)
    {
        uint_big_t tmp(lhs);
        tmp -= rhs;
        return tmp;
    }

    uint_big_t& operator *= (uint8_t num);

    uint_big_t& operator *=(const uint_big_t& rhs);

    friend uint_big_t operator*(const uint_big_t& lhs, const uint_big_t& rhs)
    {
        uint_big_t tmp(lhs);
        tmp *= rhs;
        return tmp;
    }

    uint_big_t& operator/=(const uint_big_t& rhs)
    {
        auto result = divide(*this, rhs);
        this->number_.assign(result.first.number_.begin(), result.first.number_.end());
        return *this;
    }

    uint_big_t& operator%=(const uint_big_t& rhs)
    {
        auto result = divide(*this, rhs);
        this->number_.assign(result.second.number_.begin(), result.second.number_.end());
        return *this;
    }

    friend uint_big_t operator/(const uint_big_t& lhs, const uint_big_t& rhs)
    {
        uint_big_t tmp(lhs);
        tmp /= rhs;
        return tmp;
    }

    friend uint_big_t operator%(const uint_big_t& lhs, const uint_big_t& rhs)
    {
        uint_big_t tmp(lhs);
        tmp %= rhs;
        return tmp;
    }

    friend std::istream& operator >> (std::istream& in, uint_big_t& num)
    {
        std::string strnum;
        in >> strnum;
        num.number_.resize(strnum.length());
        std::transform(strnum.rbegin(), strnum.rend(), num.number_.begin(), [](char ch)
        {
            return ch - '0';
        });
        return in;
    }

    friend std::ostream& operator << (std::ostream& out, const uint_big_t& num)
    {
        std::for_each(num.number_.rbegin(), num.number_.rend(), [&](uint8_t d)
        {
            out << static_cast<char>(d + '0');
        });
        return out;
    }

    static std::pair<uint_big_t, uint_big_t> divide(const uint_big_t& lhs, const uint_big_t& rhs);

private:
    std::vector<uint8_t> number_;

    void normalize()
    {
        while (number_.size() > 1 && number_[number_.size() - 1] == 0)
        {
            number_.resize(number_.size() - 1);
        }
    }

    uint_big_t(std::vector<uint8_t>& rhs) : number_()
    {
        number_.assign(rhs.begin(), rhs.end());
        normalize();
    }

};

std::pair<uint_big_t, uint_big_t> uint_big_t::divide(const uint_big_t& lhs, const uint_big_t& rhs)
{
    if (rhs == 0) { throw "Illegal operation"; }

    std::pair<uint_big_t, uint_big_t> result;

    if (rhs == 1)
    {
        result.first.number_.assign(lhs.number_.begin(), lhs.number_.end());
        result.second = 0;
    }
    else if (lhs < rhs)
    {
        result.first = 0;
        result.second.number_.assign(lhs.number_.begin(), lhs.number_.end());
    }
    else if (lhs == rhs)
    {
        result.first = 1;
        result.second = 0;
    }
    else
    {
        uint_big_t _lhs(lhs);

        while (rhs <= _lhs)
        {
            uint_big_t tmp;
            tmp.number_.resize(rhs.number_.size());
            std::copy_n(_lhs.number_.rbegin(), rhs.number_.size(), tmp.number_.rbegin());

            if (tmp < rhs)
            {
                tmp.number_.resize(rhs.number_.size() + 1);
                std::copy_backward(tmp.number_.begin(), tmp.number_.end() - 1, tmp.number_.end());
                tmp.number_[0] = *(_lhs.number_.rbegin() + rhs.number_.size());
            }

            uint8_t counter(0);
            size_t orig(tmp.number_.size());

            while (rhs < tmp || rhs == tmp)
            {
                tmp -= rhs;
                counter++;
            }

            result.first.number_.push_back(counter);
            auto rbegin = _lhs.number_.begin();
            rbegin += _lhs.number_.size() - orig + tmp.number_.size();
            _lhs.number_.erase(rbegin, _lhs.number_.end());
            std::copy_backward(tmp.number_.begin(), tmp.number_.end(), _lhs.number_.end());

            if (std::accumulate(_lhs.number_.begin(), _lhs.number_.end(), true, [](bool prev, uint8_t d) { return prev && d == 0;}))
            {
                result.first.number_.resize(result.first.number_.size() + _lhs.number_.size() - 1);
                break;
            }
            _lhs.normalize();
        }

        std::reverse(result.first.number_.begin(), result.first.number_.end());
        result.second.number_.assign(_lhs.number_.begin(), _lhs.number_.end());
        result.first.normalize();
        result.second.normalize();
    }

    return result;
}

bool uint_big_t::operator < (const uint_big_t& rhs) const
{
    if (number_.size() == rhs.number_.size())
    {
        auto ri = rhs.number_.rbegin();

        for (auto li = number_.rbegin(); li != number_.rend() && ri != rhs.number_.rend(); ++li, ++ri)
        {
            if (*li != *ri) { return *li < *ri; }
        }

        return false;
    }

    return number_.size() < rhs.number_.size();
}

bool uint_big_t::operator == (const uint_big_t& rhs) const
{
    if (number_.size() == rhs.number_.size())
    {
        auto ri = rhs.number_.begin();

        for (auto li = number_.begin(); li != number_.end() && ri != rhs.number_.end(); ++li, ++ri)
        {
            if (*li != *ri) { return false; }
        }

        return true;
    }

    return false;
}

uint_big_t& uint_big_t::operator+=(const uint_big_t& rhs)
{
    std::vector<uint8_t> rhs_n(rhs.number_.begin(), rhs.number_.end());
    rhs_n.resize(std::max(number_.size(), rhs.number_.size()) + 1);

    uint8_t carry(0);
    number_.resize(rhs_n.size());
    std::transform(number_.begin(), number_.end(), rhs_n.begin(), number_.begin(), [&](uint8_t lc, uint8_t rc)
    {
        uint8_t digit = lc + rc + carry;
        carry = digit / 10;
        digit %= 10;
        return digit;
    });

    normalize();

    return *this;
}

uint_big_t& uint_big_t::operator -= (const uint_big_t& rhs)
{
    if (rhs <= *this)
    {
        int16_t carry(0);
        auto li = number_.begin();
        auto apply_carry = [&](uint8_t corr)
        {
            if (carry + corr < 0)
            {
                *(li++) = 10 + carry; carry = -1;
            }
            else
            {
                *(li++) = corr + carry; carry = 0;
            }
        };
        std::for_each(rhs.number_.begin(), rhs.number_.end(), [&](uint8_t digit)
        {
            carry += *li - digit;
            apply_carry(0);
        });

        while (carry < 0 && li != number_.end())
        {
            apply_carry(*li);
        }

        normalize();
        return *this;
    }

    throw "Illegal operation";
}

uint_big_t& uint_big_t::operator *= (uint8_t num)
{
    if (num > 9) { throw "Illegal operation"; }

    number_.resize(number_.size() + 1);
    uint16_t carry(0);
    std::transform(number_.begin(), number_.end(), number_.begin(), [&](uint8_t digit)
    {
        carry += digit * num;
        digit = carry % 10;
        carry /= 10;
        return digit;
    });
    normalize();
    return *this;
}

uint_big_t& uint_big_t::operator *=(const uint_big_t& rhs)
{
    uint_big_t result;
    std::vector<uint8_t> rhs_n(rhs.number_.begin(), rhs.number_.end());

    for (auto num = rhs_n.begin(); num != rhs_n.end(); ++num)
    {
        uint_big_t lhs(*this);
        auto shift = std::distance(rhs_n.begin(), num);

        if (shift > 0)
        {
            lhs.number_.resize(lhs.number_.size() + shift);
            std::copy_backward(lhs.number_.begin(), lhs.number_.end() - shift, lhs.number_.end());
            std::fill_n(lhs.number_.begin(), shift, 0);
        }

        lhs *= *num;
        result += lhs;
    }

    this->number_.assign(result.number_.begin(), result.number_.end());
    normalize();
    return *this;
}

}  // namespace math
