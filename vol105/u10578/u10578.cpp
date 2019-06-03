#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10578.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <cstring>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10578 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : input_(), dp_(), a_win_()
        {
        }

        operator bool()
        {
            return not input_.empty();
        }

        solution_t& operator()()
        {
            std::array<uint8_t, 6> v({0, 0, 0, 0, 0, 0});
            int16_t num(0);

            for (char ch : input_)
            {
                num++;
                v[ch - '0' - 1]++;
            }

            a_win_ = not play(num % 2, v);
            return *this;
        }

        friend std::istream& operator>>(std::istream& in, solution_t& sol)
        {
            sol.input_.clear();
            in >> sol.input_;
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol)
        {
            out << sol.input_ << ' ' << (sol.a_win_ ? 'A' : 'B');
            return out;
        }

    private:
        std::string input_;
        std::map<std::string, int8_t> dp_;
        bool a_win_;

        int16_t play(int16_t p_num, std::array<uint8_t, 6>& v)
        {
            std::string cache_key;
            {
                int16_t idx(0);
                auto crc(std::accumulate(v.begin(), v.end(), 0, [&](int16_t prev, int8_t next)
                {
                    idx++;
                    cache_key += '0' + next;
                    return prev + next * idx;
                }));

                if (crc > 31)
                {
                    dp_[cache_key] = p_num;
                    return p_num;
                }
            }

            {
                auto prev = dp_.find(cache_key);

                if (prev != dp_.end())
                {
                    return prev->second;
                }
            }

            int16_t win = 1 ^ p_num;

            for (auto& vi : v)
            {
                vi++;
                int16_t tmp = play(1 ^ p_num, v);
                vi--;

                if (tmp != win)
                {
                    win = tmp;
                    break;
                }
            }

            dp_[cache_key] = win;
            return win;
        }
    };

}

void U10578::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
