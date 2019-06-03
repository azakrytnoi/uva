#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u741.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U741 instance;
    instance();
}

namespace {
    class solution {
    public:
        solution() : message_(), position_(), next_() {}

        solution& operator()()
        {
            std::string temp(message_);
            std::sort(temp.begin(), temp.end());
            next_.clear();
            next_.resize(temp.size());
            size_t last_pos(std::numeric_limits<size_t>::max());

            for (size_t idx = 0; idx < temp.size(); idx++)
            {
                if (last_pos == std::numeric_limits<size_t>::max())
                {
                    auto pos = message_.find(temp[idx]);
                    next_[idx] = last_pos = pos;
                }
                else
                {
                    if (temp[idx] == temp[idx - 1])
                    {
                        auto pos = message_.find(temp[idx], last_pos + 1);
                        last_pos = std::numeric_limits<size_t>::max();
                        next_[idx] = last_pos = pos;
                    }
                    else
                    {
                        auto pos = message_.find(temp[idx]);
                        next_[idx] = last_pos = pos;
                    }
                }
            }

            return *this;
        }

        operator bool ()
        {
            return not(message_ == "END" && position_ == 0);
        }

        friend std::istream& operator >> (std::istream& in, solution& sol)
        {
            in >> sol.message_ >> sol.position_;
            return in;
        }

        friend std::ostream& operator << (std::ostream& out, const solution& sol)
        {
            size_t pos = sol.position_ - 1;

            for (size_t idx = 0; idx < sol.message_.size(); idx++)
            {
                pos = sol.next_[pos];
                out << sol.message_[pos];
            }

            out << std::endl;
            return out;
        }

    private:
        std::string message_;
        uint16_t position_;
        std::vector<size_t> next_;
    };
}

void U741::operator()() const
{
    solution sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
