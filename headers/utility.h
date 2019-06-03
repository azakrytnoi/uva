/*
 * utility.h
 *
 *  Created on: Mar 7, 2019
 *      Author: aleksey
 */

#pragma once

#include <iterator>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

template<typename T>
struct flow_translator_t : public std::iterator<std::forward_iterator_tag, T>
{
    T& ref_;

    explicit flow_translator_t(T& ref) : ref_(ref) {}
    virtual ~flow_translator_t() {}

    flow_translator_t& operator* ()
    {
        return *this;
    }

    virtual flow_translator_t& operator = (char ch) = 0;

    flow_translator_t& operator++()
    {
        return *this;
    }
};

template<typename T>
struct hex_translator_t : public flow_translator_t<T>
{

    explicit hex_translator_t(T& ref) : flow_translator_t<T>(ref) {}
    ~hex_translator_t() {}

    hex_translator_t& operator = (char ch)
    {
        ch = std::toupper(ch);
        this->ref_ <<= 4;
        this->ref_ += ch >= 'A' ? 10 + ch - 'A' : ch - '0';
        return *this;
    }
};

template<typename T>
struct dec_translator_t : public std::iterator<std::forward_iterator_tag, T>
{

    explicit dec_translator_t(T& ref) : flow_translator_t<T>(ref) {}
    ~dec_translator_t() {}

    dec_translator_t& operator = (char ch)
    {
        this->ref_ *= 10;
        this->ref_ += ch - '0';
        return *this;
    }
};

template<typename T>
struct oct_translator_t : public std::iterator<std::forward_iterator_tag, T>
{

    explicit oct_translator_t(T& ref) : flow_translator_t<T>(ref) {}
    ~oct_translator_t() {}

    oct_translator_t& operator = (char ch)
    {
        this->ref_ <<= 3;
        this->ref_ += ch - '0';
        return *this;
    }
};

template<typename T>
struct bin_translator_t : public std::iterator<std::forward_iterator_tag, T>
{

    explicit bin_translator_t(T& ref) : flow_translator_t<T>(ref) {}
    ~bin_translator_t() {}

    bin_translator_t& operator = (char ch)
    {
        this->ref_ <<= 1;
        this->ref_ += ch - '0';
        return *this;
    }
};

#pragma GCC diagnostic pop
