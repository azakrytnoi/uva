#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u704.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <queue>
#include <deque>
#include <unordered_map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U704 instance;
    instance();
}

namespace {
    struct hash {
        size_t operator () (const std::pair<uint64_t, uint64_t>& key) const
        {
            return  key.first + key.second;
        }
    };

    enum class rotation : uint8_t {
        unknown = 0,
        left_clockwise = 1, // left wheel clockwise
        right_clockwise = 2, // right wheel clockwise
        left_counterclockwise = 3, // left wheel counterclockwise
        right_counterclockwise = 4, // right wheel counterclockwise
    };

    typedef std::pair<uint64_t, uint64_t> wheels;
    typedef std::unordered_map<const wheels, std::deque<rotation>, hash> WheelMap;

    const size_t numbers_per_wheel = 12;
    const size_t max_movements = 16; // max. number of movements
    const wheels wheels_solved(0x034305650121, 0x078709a90121);

    wheels rotate_wheel(rotation direction, const wheels& wheel)
    {
        const uint64_t mask_8_bits = 0xff, mask_12_bits = 0xfff;
        uint64_t left = wheel.first, right = wheel.second;

        switch (direction) {
        case rotation::left_clockwise: {
            uint64_t temp = left & mask_8_bits;
            left >>= 8;
            left |= temp << 40;
            right &= ~mask_12_bits;
            right |= left & mask_12_bits;
            break;
        }

        case rotation::right_clockwise: {
            uint64_t temp = (right & mask_8_bits << 40) >> 40;
            right &= ~(mask_8_bits << 40);
            right <<= 8;
            right |= temp;
            left &= ~mask_12_bits;
            left |= right & mask_12_bits;
            break;
        }

        case rotation::left_counterclockwise: {
            uint64_t temp = (left & mask_8_bits << 40) >> 40;
            left &= ~(mask_8_bits << 40);
            left <<= 8;
            left |= temp;
            right &= ~mask_12_bits;
            right |= left & mask_12_bits;
            break;
        }

        case rotation::right_counterclockwise: {
            uint64_t temp = right & mask_8_bits;
            right >>= 8;
            right |= temp << 40;
            left &= ~mask_12_bits;
            left |= right & mask_12_bits;
            break;
        }
        }

        return wheels(left, right);
    }

    class solution {
        WheelMap lower_half_cache_;
        std::deque<rotation> movements_;
        wheels wheels_;
        bool solved_;
    public:
        solution()
        {
            generate_cache();
        }

        friend std::istream& operator >> (std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        void generate_cache();
        bool rotate_wheel_and_cache(rotation direction, const wheels& wheel, const std::deque<rotation>& movements,
                                    std::queue<wheels>& working_q);
        bool rotate_wheel_and_cache(rotation direction, const wheels& wheel, const std::deque<rotation>& movements,
                                    std::queue<wheels>& work_q, WheelMap& cache);
    };

    void solution::generate_cache()
    {
        lower_half_cache_.insert(std::make_pair(wheels_solved, std::deque<rotation>()));
        std::queue<wheels> bfs_q;
        bfs_q.push(wheels_solved);

        while (!bfs_q.empty()) {
            wheels wheel = bfs_q.front();
            bfs_q.pop();
            std::deque<rotation> movements = lower_half_cache_[wheel];

            if (movements.size() < max_movements / 2) {
                for (int32_t direction = static_cast<int32_t>(rotation::left_clockwise); direction <= static_cast<int32_t>(rotation::right_counterclockwise); direction++) {
                    rotate_wheel_and_cache(static_cast<rotation>(direction), wheel, movements, bfs_q);
                }
            }
        }
    }

    bool solution::rotate_wheel_and_cache(rotation direction, const wheels& wheel, const std::deque<rotation>& movements,
                                          std::queue<wheels>& working_q)
    {
        wheels new_wheel = rotate_wheel(direction, wheel);

        if (lower_half_cache_.find(new_wheel) != lower_half_cache_.end()) {
            return false;
        }

        rotation new_dir;

        switch (direction) {
        case rotation::left_clockwise:
            new_dir = rotation::left_counterclockwise;
            break;

        case rotation::right_clockwise:
            new_dir = rotation::right_counterclockwise;
            break;

        case rotation::left_counterclockwise:
            new_dir = rotation::left_clockwise;
            break;

        case rotation::right_counterclockwise:
            new_dir = rotation::right_clockwise;
            break;
        }

        std::deque<rotation> new_movements(movements);
        new_movements.push_front(new_dir);
        lower_half_cache_.insert(std::make_pair(new_wheel, new_movements));
        working_q.push(new_wheel);
        return true;
    }

    bool solution::rotate_wheel_and_cache(rotation direction, const wheels& wheel, const std::deque<rotation>& movements,
                                          std::queue<wheels>& work_q, WheelMap& cache)
    {
        wheels new_wheel = rotate_wheel(direction, wheel);

        if (cache.find(new_wheel) != cache.end()) {
            return false;
        }

        std::deque<rotation> new_movements(movements);
        new_movements.push_back(direction);
        cache.insert(std::make_pair(new_wheel, new_movements));
        work_q.push(new_wheel);
        return true;
    }

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        int nr;

        for (size_t i = 0; i < numbers_per_wheel; i++) {
            std::cin >> nr;
            sol.wheels_.first <<= 4;
            sol.wheels_.first |= nr;
        }

        for (size_t i = 0; i < numbers_per_wheel; i++) {
            std::cin >> nr;
            sol.wheels_.second <<= 4;
            sol.wheels_.second |= nr;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        if (sol.solved_) {
            if (sol.movements_.empty()) {
                std::cout << "PUZZLE ALREADY SOLVED";

            } else {
                for (std::deque<rotation>::const_iterator it = sol.movements_.begin(), e = sol.movements_.end();
                        it != e; ++it) {
                    std::cout << static_cast<int32_t>(*it);
                }
            }

        } else {
            std::cout << "NO SOLUTION WAS FOUND IN 16 STEPS";
        }

        return out;
    }

    solution& solution::operator()()
    {
        solved_ = false;
        WheelMap cache;
        cache.insert(std::make_pair(wheels_, std::deque<rotation>()));
        std::queue<wheels> bfs_q;
        bfs_q.push(wheels_);
        WheelMap::const_iterator lhce = lower_half_cache_.end();

        while (!bfs_q.empty()) {
            wheels wheel = bfs_q.front();
            bfs_q.pop();
            std::deque<rotation>& movements = cache[wheel];
            WheelMap::const_iterator lhci = lower_half_cache_.find(wheel);

            if (lhci != lhce) {
                solved_ = true;
                movements_ = movements;
                movements_.insert(movements_.end(), lhci->second.begin(), lhci->second.end());
                break;

            } else if (movements.size() < max_movements / 2) {
                for (int32_t direction = static_cast<int32_t>(rotation::left_clockwise); direction <= static_cast<int32_t>(rotation::right_counterclockwise); direction++) {
                    rotate_wheel_and_cache(static_cast<rotation>(direction), wheel, movements, bfs_q, cache);
                }
            }
        }

        return *this;
    }
}

void U704::operator()() const
{
    int N;
    std::cin >> N;
    solution sol;

    while (N--) {
        std::cin >> sol;
        std::cout << sol() << std::endl;
    }
}
