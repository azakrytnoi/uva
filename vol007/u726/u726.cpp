#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u726.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <cctype>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U726 instance;
    instance();
}

namespace {

    class solution {
    public:
        solution() : known_(), encoded_(), decoded_() { }

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        operator bool() const
        {
            return true;
        }
        solution& operator()();

    private:
        std::string known_;
        std::string encoded_;
        std::string decoded_;
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        std::string line;

        while (std::getline(in, line) && not line.empty()) {
            sol.known_ += line;
        }

        while (std::getline(in, line)) {
            sol.encoded_ += line;
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        out << sol.decoded_;
        return out;
    }

    solution& solution::operator()()
    {
        typedef std::map<char, uint16_t> freq_map;
        struct freq_item : std::pair<freq_map::key_type, freq_map::mapped_type> {
            freq_item(const std::pair<const freq_map::key_type, freq_map::mapped_type>& val) //
                : std::pair<freq_map::key_type, freq_map::mapped_type>(val.first, val.second) {}

            freq_item& operator=(const freq_item& rhs)
            {
                first = rhs.first;
                second = rhs.second;
                return *this;
            }

            bool operator < (const freq_item& rhs)
            {
                if (first == rhs.first) {
                    return second < rhs.second;
                }

                return first < rhs.first;
            }
        };
        typedef std::vector<freq_item> freq_vector;

        freq_vector zero;
        zero.reserve('z' - 'a' + 1);
        std::generate_n(std::back_inserter(zero), 'z' - 'a' + 1, []() {
            static char key ('a');
            return freq_item(std::make_pair(key++, 0));
        });

        freq_map freq_known;
        freq_known.insert(zero.begin(), zero.end());

        for (char ch : known_) {
            if (std::isalpha(ch)) {
                freq_known[std::tolower(ch)]++;
            }
        }

        freq_map freq_encoded;
        freq_encoded.insert(zero.begin(), zero.end());

        for (char ch : encoded_) {
            if (std::isalpha(ch)) {
                freq_encoded[std::tolower(ch)]++;
            }
        }

        freq_vector known_chars(freq_known.begin(), freq_known.end());
        std::sort(known_chars.begin(), known_chars.end(), [](const freq_item & lhs, const freq_item & rhs) {
            if (lhs.second != rhs.second) {
                return lhs.second < rhs.second;
            }

            return lhs.first < rhs.first;
        });
        freq_vector encoded (freq_encoded.begin(), freq_encoded.end());
        std::sort(encoded.begin(), encoded.end(), [](const freq_item & lhs, const freq_item & rhs) {
            if (lhs.second != rhs.second) {
                return lhs.second < rhs.second;
            }

            return lhs.first < rhs.first;
        });
        std::map<char, char> coding_map;

        for (auto known_it = known_chars.begin(), enc_it = encoded.begin(); //
                known_it != known_chars.end() && enc_it != encoded.end(); //
                ++known_it, ++ enc_it) {
            coding_map[enc_it->first] = known_it->first;
        }

        std::transform(encoded_.begin(), encoded_.end(), std::back_inserter(decoded_), [&](char ch) -> char {
            if (not std::isalpha(ch))
            {
                return ch;
            }

            char result = coding_map[std::tolower(ch)];
            return std::isupper(ch) ? std::toupper(result) : result;
        });
        return *this;
    }

}

void U726::operator()() const
{
    solution sol;
    std::cin >> sol;
    std::cout << sol() << std::endl;
}
