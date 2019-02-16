#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u409.h"

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
    U409 instance;
    instance();
}

namespace {

    class solution_t {
    public:
        solution_t() : case_no_(), keywords_(), excuses_(), worst_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        size_t case_no_;
        std::vector<std::string> keywords_;
        std::vector<std::string> excuses_;
        std::vector<std::string> worst_;
    };

    solution_t& solution_t::operator()()
    {
        case_no_++;
        typedef std::pair<size_t, std::string> freq_t;
        std::vector<freq_t> freq;
        freq.reserve(excuses_.size());

        for (auto excuse : excuses_) {
            auto cnt(std::make_pair(0, excuse));
            std::transform(excuse.begin(), excuse.end(), excuse.begin(), [](const char ch) {
                return std::isalpha(ch) ? std::tolower(ch) : ' ';
            });
            std::copy_if(excuse.begin(), excuse.end(), excuse.begin(), [](const char ch) {
                return ch == ' ' || std::islower(ch);
            });
            std::vector<std::string> parsed;
            std::stringstream src(excuse);
            std::istream_iterator<std::string> isrc(src);
            std::copy(isrc, std::istream_iterator<std::string>(), std::back_inserter(parsed));
            parsed.shrink_to_fit();

            for (auto keyword : keywords_) {
                cnt.first += std::count(parsed.begin(), parsed.end(), keyword);
            }

            freq.push_back(cnt);
        }

        std::sort(freq.begin(), freq.end(), [](const freq_t& lhs, const freq_t& rhs) {
            return lhs.first > rhs.first;
        });
        size_t max_cnt(freq.front().first);
        std::vector<freq_t> max_freq;
        max_freq.reserve(freq.size());
        std::copy_if(freq.begin(), freq.end(), std::back_inserter(max_freq), [&](const freq_t& val) {
            return val.first == max_cnt;
        });
        worst_.reserve(max_freq.size());
        std::transform(max_freq.begin(), max_freq.end(), std::back_inserter(worst_), [](const freq_t& val) {
            return val.second;
        });
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.keywords_.clear();
        sol.excuses_.clear();
        sol.worst_.clear();
        size_t n(0), m(0);

        if (in >> n >> m) {
            std::istream_iterator<std::string> kin(in);
            sol.keywords_.reserve(n);
            std::copy_n(kin, n, std::back_inserter(sol.keywords_));
            in.ignore();
            std::string line;
            sol.excuses_.reserve(m);
            std::generate_n(std::back_inserter(sol.excuses_), m, [&]() {
                std::getline(in, line);
                return line;
            });
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << "Excuse Set #" << sol.case_no_ << std::endl;
        std::ostream_iterator<std::string> eout(out, "\n");
        std::copy(sol.worst_.begin(), sol.worst_.end(), eout);
        return out;
    }

}

void U409::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol) {
        std::cout << sol() << std::endl;
    }
}
