/*
 * rational.h
 *
 *  Created on: Mar 21, 2019
 *      Author: aleksey
 */

#pragma once

#include <vector>
#include <cmath>

#include "gcd.h"

namespace math {

    template<typename T>
    class rational_t {
        T a_;
        T b_;

    public:
        rational_t(const T& a = 0, const T& b = 1) : a_(a), b_(b)
        {
            normalize();
        }
        rational_t(T&& a = 0, T && b = 1) : a_(std::move(a)), b_(std::move(b))
        {
            normalize();
        }
        rational_t(const rational_t& r) : a_(r.a_), b_(r.b_)
        {
            normalize();
        }
        rational_t(T&& r) : a_(std::move(r.a_)), b_(std::move(r.b_))
        {
            normalize();
        }

        rational_t& operator = (const T& a)
        {
            a_ = a;
            b_ = 1;
            return *this;
        }

        friend
        bool operator == (const rational_t& lhs, const rational_t& rhs)
        {
            return lhs.a_ * rhs.b_ == lhs.b_ * rhs.a_;
        }

        friend
        bool operator < (const rational_t& lhs, const rational_t& rhs)
        {
            return lhs.a_ * rhs.b_ < lhs.b_ * rhs.a_;
        }

        rational_t& operator +=(const rational_t& rhs)
        {
            auto aprim(a_ * rhs.b_ + b_ * rhs.a_);
            auto bprim(b_ * rhs.b_);
            a_ = aprim, b_ = bprim;
            normalize();
            return *this;
        }

        friend
        rational_t operator +(const rational_t& lhs, const rational_t& rhs)
        {
            rational_t tmp(lhs);
            tmp += rhs;
            return tmp;
//            return rational_t((lhs.a_ * rhs.b_ + lhs.b_ * rhs.a_), (lhs.b_ * rhs.b_));
        }

        rational_t& operator -=(const rational_t& rhs)
        {
            auto aprim = a_ * rhs.b_ - b_ * rhs.a_;
            auto bprim = b_ * rhs.b_;
            a_ = aprim, b_ = bprim;
            normalize();
            return *this;
        }

        friend
        rational_t operator -(const rational_t& lhs, const rational_t& rhs)
        {
            rational_t tmp(lhs);
            tmp -= rhs;
            return tmp;
//            return rational_t((lhs.a_ * rhs.b_ - lhs.b_ * rhs.a_), (lhs.b_ * rhs.b_));
        }

        rational_t& operator *=(const rational_t& rhs)
        {
            auto aprim(a_ * rhs.a_);
            auto bprim(b_ * rhs.b_);
            a_ = aprim, b_ = bprim;
            normalize();
            return *this;
        }

        friend
        rational_t operator *(const rational_t& lhs, const rational_t& rhs)
        {
            rational_t tmp(lhs);
            tmp *= rhs;
            return tmp;
//            return rational_t(lhs.a_ * rhs.a_, lhs.b_ * rhs.b_);
        }

        rational_t& operator /=(const rational_t& rhs)
        {
            auto aprim(a_ * rhs.b_);
            auto bprim(b_ * rhs.a_);
            a_ = aprim, b_ = bprim;
            normalize();
            return *this;
        }

        friend
        rational_t operator /(const rational_t& lhs, const rational_t& rhs)
        {
            rational_t tmp(lhs);
            tmp *= rhs;
            return tmp;
//            return rational_t(lhs.a_ * rhs.b_, lhs.b_ * rhs.a_);
        }

        rational_t& reciprocal()
        {
            std::swap(a_, b_);
            return *this;
        }

        std::vector<T> fraction() const;

    private:
        void normalize()
        {
            if (b_ == 0)
            {
                throw "Illegal number";
            }

            auto div = gcd(a_, b_);
            a_ /= div;
            b_ /= div;
        }
    };

    template<typename T>
    std::vector<T> rational_t<T>::fraction() const
    {
        std::vector<T> result;
        auto temp(*this);

        while (temp.b_ != 1)
        {
            auto int_part (temp.a_ / temp.b_);
            result.push_back(int_part);
            temp -= int_part;
            temp.reciprocal();
        }

        result.push_back(temp.a_);
        result.shrink_to_fit();
        return result;
    }

}  // namespace math
