#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10044.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>
#include <regex>
#include <list>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10044 instance;
    instance();
}

namespace {
    class solution {
        std::map<std::string, std::list<std::string>> coauthors_;
        std::map<std::string, uint32_t> enumbers_;
        std::vector<std::string> candidates_;
    public:
        solution();

        friend std::istream& operator >>(std::istream& in, solution& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution& sol);

        solution& operator()();

    private:
        uint32_t calculate(const std::string& candidate, std::list<std::string>& passed);
    };

    std::istream& operator >> (std::istream& in, solution& sol)
    {
        sol.enumbers_.clear();
        sol.coauthors_.clear();
        uint32_t n_books, n_authors;
        in >> n_books >> n_authors;
        std::string line;
        std::getline(in, line);

        while (n_books--) {
            std::getline(in, line);
            std::string temp = line.substr(0, line.find(':'));
            temp.erase(temp.find_last_not_of(' ') + 1);
            std::list<std::string> coauthors;

            while (temp.find(".,") != std::string::npos) {
                std::string author = temp.substr(0, temp.find(".,") + 1);
                temp.erase(0, author.length() + 1);
                author.erase(0, author.find_first_not_of(' '));
                coauthors.push_back(author);
                sol.enumbers_[author] = std::numeric_limits<uint32_t>::max();
            }

            temp.erase(0, temp.find_first_not_of(' '));
            coauthors.push_back(temp);
            sol.enumbers_[temp] = std::numeric_limits<uint32_t>::max();
            std::for_each(coauthors.begin(), coauthors.end(), [&](const std::string & author) {
                std::copy_if(coauthors.begin(), coauthors.end(), std::back_inserter(sol.coauthors_[author]), [&](auto coauthor) {
                    return author != coauthor
                           && std::find(sol.coauthors_[author].begin(), sol.coauthors_[author].end(), coauthor) == sol.coauthors_[author].end();
                });
            });
        }

        sol.candidates_.clear();
        sol.candidates_.reserve(n_authors);
        std::generate_n(std::back_inserter(sol.candidates_), n_authors, [&]() {
            std::getline(in, line);
            return line;
        });
        sol.enumbers_["Erdos, P."] = 0;
        return in;
    }

    solution& solution::operator ()()
    {
        std::for_each(candidates_.begin(), candidates_.end(), [&](const std::string & candidate) {
            if (enumbers_[candidate] == std::numeric_limits<uint32_t>::max()) {
                std::list<std::string> trace;
                enumbers_[candidate] = calculate(candidate, trace);
            }
        });
        return *this;
    }

    solution::solution() : coauthors_(), enumbers_(), candidates_() {}

    std::ostream& operator << (std::ostream& out, const solution& sol)
    {
        std::for_each(sol.candidates_.begin(), sol.candidates_.end(), [&](const std::string & author) {
            out << author << " ";
            uint32_t enumber = sol.enumbers_.find(author)->second;

            if (enumber == std::numeric_limits<uint32_t>::max()) {
                out << "infinity" << std::endl;

            } else {
                out << enumber << std::endl;
            }
        });
        return out;
    }

    uint32_t solution::calculate(const std::string& candidate, std::list<std::string>& passed)
    {
        if (enumbers_[candidate] != std::numeric_limits<uint32_t>::max()) {
            return enumbers_[candidate];
        }

        if (std::find(passed.begin(), passed.end(), candidate) == passed.end()) {
            passed.push_back(candidate);
            uint32_t min_number(std::accumulate(coauthors_[candidate].begin(), coauthors_[candidate].end(), std::numeric_limits<uint32_t>::max(), [&](uint32_t running, const std::string & name) {
                return std::min(running, calculate(name, passed));
            }));

            if (min_number != std::numeric_limits<uint32_t>::max()) {
                return min_number + 1;
            }
        }

        return std::numeric_limits<uint32_t>::max();
    }
}

void U10044::operator()() const
{
    uint32_t N;
    std::cin >> N;
    solution sol;
    uint32_t nCase(0);

    while (N--) {
        std::cin >> sol;
        std::cout << "Scenario " << (++nCase) << std::endl << sol() << std::endl;
    }
}
