#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10008.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10008 instance;
    instance();
}

namespace {
    struct freq_t
    {
        char ch_;
        size_t freq_;

        freq_t(char ch = 0, size_t freq = 0) : ch_(ch), freq_(freq) {}
        freq_t(const std::pair<const char, size_t>& val) : ch_(val.first), freq_(val.second) {}

        friend std::ostream& operator <<(std::ostream& out, const freq_t& freq)
        {
            out << freq.ch_ << ' ' << freq.freq_;
            return out;
        }
    };

    class solution_t {
    public:
        solution_t() : result_(), source_() {}

        operator bool()
        {
            return true;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        std::vector<freq_t> result_;
        std::string source_;
    };

    class freq_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
    protected:
        std::map<char, size_t>* container;

    public:
        typedef std::map<char, size_t> container_type;

        explicit freq_insert_iterator(container_type& __x) : container(&__x) {}

        freq_insert_iterator&
        operator=(typename container_type::key_type __value)
        {
            auto val(std::toupper(__value));
            auto it = container->find(val);

            if (it == container->end())
            {
                it = container->insert(it, std::make_pair(val, 0));
            }

            ++it->second;
            return *this;
        }

        freq_insert_iterator& operator*()
        {
            return *this;
        }

        freq_insert_iterator& operator++()
        {
            return *this;
        }

        freq_insert_iterator operator++(int)
        {
            return *this;
        }
    };

    inline freq_insert_iterator freq_inserter(freq_insert_iterator::container_type& __x)
    {
        return freq_insert_iterator(__x);
    }

    solution_t& solution_t::operator()()
    {
        std::map<char, size_t> temp;
        std::copy_if(source_.begin(), source_.end(), freq_inserter(temp), [](char ch)
        {
            return std::isalpha(ch);
        });
        result_.reserve(temp.size());
        std::copy(temp.begin(), temp.end(), std::back_inserter(result_));
        std::sort(result_.begin(), result_.end(), [](const freq_t& lhs, const freq_t& rhs)
        {
            if (lhs.freq_ == rhs.freq_)
            {
                return lhs.ch_ < rhs.ch_;
            }

            return lhs.freq_ > rhs.freq_;
        });
        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        size_t n(0);
        std::string line;

        if (in >> n && n > 0)
        {
            in.ignore();

            while (n--)
            {
                std::getline(in, line);
                sol.source_ += line;
            }
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        std::ostream_iterator<freq_t> fout(out, "\n");
        std::copy(sol.result_.begin(), sol.result_.end(), fout);
        return out;
    }
}

void U10008::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
