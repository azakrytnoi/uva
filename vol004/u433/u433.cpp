#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u433.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <cmath>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U433 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(std::numeric_limits<size_t>::max()), digits_({
            {" _ | ||_|", 0},
            {"     |  |", 1},
            {" _  _||_ ", 2},
            {" _  _| _|", 3},
            {"   |_|  |", 4},
            {" _ |_  _|", 5},
            {" _ |_ |_|", 6},
            {" _   |  |", 7},
            {" _ |_||_|", 8},
            {" _ |_| _|", 9},
        }), readings_(), corrupted_(), result_(), rescnt_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return case_no_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t case_no_;
        const std::map<std::string, uint8_t> digits_;
        std::vector<std::string> readings_;
        size_t corrupted_;
        int64_t result_;
        size_t rescnt_;

        void evaluate();
        bool is_similar (const std::string& digit, const std::string& reading);
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.case_no_ == std::numeric_limits<size_t>::max()) {
            in >> sol.case_no_;
            in.ignore();
        }

        std::vector<std::string> readings(3);

        for (auto& line : readings) {
            if (not std::getline(in, line)) {
                return in;
            }
        }

        sol.readings_.clear();
        sol.readings_.resize(9);
        sol.corrupted_ = std::numeric_limits<size_t>::max();

        for (auto i = 0; i < 9; ++i) {
            sol.readings_[i] = readings[0].substr(i * 3, 3) + readings[1].substr(i * 3, 3) + readings[2].substr(i * 3, 3);
            auto digit = sol.digits_.find(sol.readings_[i]);

            if (digit == sol.digits_.end()) {
                sol.corrupted_ = i;
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        switch (sol.rescnt_) {
        case 1:
            out << std::setw(9) << std::setfill('0') << sol.result_;
            break;

        case 0:
            out << "failure";
            break;

        default:
            out << "ambiguous";
            break;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        result_ = -1;
        rescnt_ = 0;
        evaluate();

        return *this;
    }

    void solution_t::evaluate ()
    {
        std::deque<std::vector<std::string>> work;

        if (corrupted_ != std::numeric_limits<size_t>::max()) {
            for (auto digit = digits_.begin(); digit != digits_.end(); ++digit) {
                if (is_similar(digit->first, readings_[corrupted_])) {
                    auto item = readings_;
                    item[corrupted_] = digit->first;
                    work.push_back(item);
                }
            }
        } else {
            work.push_back(readings_);

            for (size_t idx = 0; idx < 9; ++idx) {
                for (auto digit = digits_.begin(); digit != digits_.end(); ++digit) {
                    if (digit->first != readings_[idx] && is_similar(digit->first, readings_[idx])) {
                        auto item = readings_;
                        item[idx] = digit->first;
                        work.push_back(item);
                    }
                }
            }
        }

        while (not work.empty()) {
            auto item = work.front();
            work.pop_front();
            uint64_t code(0), result(0);

            for (size_t i = 0; i < 9; ++i) {
                int8_t digit = digits_.find(item[i])->second;
                code += (9 - i) * 9 * digit;
                result *= 10;
                result += digit;
            }

            if (code % 11 == 0) {
                result_ = result;

                if (++rescnt_ == 2) {
                    return;
                }
            }
        }
    }

    bool solution_t::is_similar(const std::string& digit, const std::string& reading)
    {
        for (auto di = digit.begin(), ri = reading.begin(); di != digit.end() && ri != reading.end(); ++di, ++ri) {
            if (*di != *ri && *di == ' ') {
                return false;
            }
        }

        return true;
    }
}

void U433::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
