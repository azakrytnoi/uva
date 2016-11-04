#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace math {
    class BigInteger {
        std::vector<uint32_t> data_;
    public:
        BigInteger(const std::string& data = std::string("")) : data_()
        {
            data_.reserve(data.length() + 1);
            std::transform(data.rbegin(), data.rend(), std::back_inserter(data_), [](auto ch) {
                return ch - '0';
            });
            data_.push_back(0);
        }

        BigInteger& operator = (const BigInteger& rhs);

        const std::vector<uint32_t>& data() const
        {
            return data_;
        }

        friend
        std::ostream& operator << (std::ostream& out, const BigInteger& rhs);

        BigInteger& operator += (const BigInteger& rhs);
        BigInteger& operator *= (const BigInteger& rhs);
        BigInteger& operator *= (const int mult);

        size_t length() const;
    private:
    };

    inline BigInteger& BigInteger::operator=(const BigInteger& rhs)
    {
        data_.clear();
        data_.reserve(rhs.data_.size());
        std::copy(rhs.data_.begin(), rhs.data_.end(), std::back_inserter(data_));
        return *this;
    }

    inline BigInteger& BigInteger::operator+=(const BigInteger& rhs)
    {
        if (rhs.data_.size() > data_.size()) {
            data_.resize(rhs.data_.size() + 1);
        }

        auto it = data_.begin();
        std::for_each(rhs.data_.begin(), rhs.data_.end(), [&](auto d) {
            *it += d;
            int rem(*it / 10);

            if (rem > 0) {
                *it %= 10;
                *(it + 1) += rem;
            }

            ++it;
        });
        return *this;
    }

    inline BigInteger& BigInteger::operator*=(const int mult)
    {
        if (mult == 0) {
            *this = std::string("0");

        } else if (mult > 1) {
            data_.resize(data_.size() + size_t(std::log10(mult) + 0.5));

            for (auto it = data_.begin(); it != data_.end(); ++it) {
                *it *= mult;
            }

            for (auto it = data_.begin(); it != data_.end(); ++it) {
                int rem(*it / 10);

                if (rem > 0) {
                    *it %= 10;

                    if (it + 1 == data_.end()) {
                        data_.push_back(rem);
                        break;;

                    } else {
                        *(it + 1) += rem;
                    }
                }
            }
        }

        return *this;
    }

    inline BigInteger& BigInteger::operator*=(const BigInteger& rhs)
    {
        int pos(1);
        BigInteger result;
        std::for_each(rhs.data_.begin(), rhs.data_.end(), [&](auto digit) {
            BigInteger tmp(*this);
            tmp *= (digit * pos);
            result += tmp;
            pos *= 10;
        });
        *this = result;
        return *this;
    }

    inline size_t BigInteger::length() const
    {
        return data_.size();
    }

    inline std::ostream& operator<<(std::ostream& out, const BigInteger& rhs)
    {
        bool leading0(true);
        std::for_each(rhs.data_.rbegin(), rhs.data_.rend(), [&](auto ch) {
            if (leading0) {
                leading0 = (ch == 0);
            }

            if (!leading0) {
                out << char(ch + '0');
            }
        });
        return out;
    }
}
