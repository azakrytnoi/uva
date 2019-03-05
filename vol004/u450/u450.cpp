#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u450.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <memory>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U450 instance;
    instance();
}

namespace {

    class parser_t : public std::string { };
    std::istream& operator>>(std::istream& in, parser_t& out)
    {
        std::getline(in, out, ',');
        return in;
    }

    struct person_t {
        std::vector<std::string> info_;

        explicit person_t(std::string dep) : info_(8)
        {
            info_[7] = dep;
        }

        friend std::istream& operator >>(std::istream& in, person_t& person)
        {
            std::copy_n(std::istream_iterator<parser_t>(in), 7, person.info_.begin());
            return in;
        }

        friend std::ostream& operator<<(std::ostream& out, const person_t& person)
        {
            out << std::string(40, '-') << std::endl //
                << person.info_[0] << " " << person.info_[1] << " " << person.info_[2] << std::endl //
                << person.info_[3] << std::endl //
                << "Department: " << person.info_[7] << std::endl //
                << "Home Phone: " << person.info_[4] << std::endl //
                << "Work Phone: " << person.info_[5] << std::endl //
                << "Campus Box: " << person.info_[6];
            return out;
        }
    };

    class solution_t {
    public:
        solution_t() : persons_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        std::map<std::string, std::shared_ptr<person_t>> persons_;
    };

    solution_t& solution_t::operator()()
    {
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        size_t n_dep(0);

        if (in >> n_dep) {
            std::string dep;
            std::getline(in, dep);

            while (n_dep--) {
                std::getline(in, dep);
                std::string line;

                while (std::getline(in, line) && not line.empty()) {
                    auto person = std::make_shared<person_t>(dep);
                    std::stringstream sin(line);
                    sin >> *person;
                    sol.persons_.insert(std::make_pair(person->info_[2], person));
                }
            }
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        std::for_each(sol.persons_.begin(), sol.persons_.end(), [&](const std::pair<std::string, std::shared_ptr<person_t>>& item) {
            out << *item.second << std::endl;
        });
        return out;
    }

}

void U450::operator()() const
{
    solution_t sol;

    std::cin >> sol;
    std::cout << sol() << std::endl;
}
