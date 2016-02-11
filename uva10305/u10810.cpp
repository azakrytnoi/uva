#include "u10810.h"

#include <list>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

namespace
{
std::ostream_iterator<int> log (std::clog, " ");

class sorter
{
public:
    sorter(int n);
    ~sorter() {}

    uint64_t sort();

    friend std::istream& operator >> (std::istream& in, sorter& s)
    {
        int n = s.n_;
        while (n--)
        {
            int val;
            std::cin >> val;
            s.src_.push_back(val);
        }
        return in;
    }

protected:
private:
    typedef std::list<int>::iterator iterator;
    int n_;
    uint64_t swaps_;
    std::list<int> src_;

    void merge_sort (iterator, iterator);
    void merge (iterator, iterator, iterator);
};

sorter::sorter(int n) : n_(n), swaps_(0)
{
}

uint64_t sorter::sort()
{
    merge_sort(src_.begin(), src_.end());
    return swaps_;
}

void sorter::merge_sort (iterator start, iterator end)
{
    std::clog << "merge-sort [ ";
    std::copy(start, end, log);
    std::clog << "]" << std::endl;
    if ( std::distance(start, end) > 1 )
    {
        iterator middle ( start);
        std::advance(middle, std::distance(start, end) / 2);
        merge_sort (start, middle);
        merge_sort (middle, end);
        merge (start, middle, end);
        std::clog << "merge-sort after [ ";
        std::copy(start, end, log);
        std::clog << "]" << std::endl;
    }
}

void sorter::merge (iterator start, iterator middle, iterator end)
{
    std::clog << "merge [ ";
    std::copy(start, middle, log);
    std::clog << "/ ";
    std::copy(middle, end, log);
    std::clog << "]" << std::endl;
    std::list <int> left;
    std::list <int> right;

    std::copy(start, middle, std::back_inserter(left));
    std::copy(middle, end, std::back_inserter(right));

    left.push_back (999999999 + 2);
    right.push_back (999999999 + 2);
    std::clog << "\tleft [ ";
    std::copy (left.begin(), left.end(), log);
    std::clog << "]" << std::endl;
    std::clog << "\tright [ ";
    std::copy (right.begin(), right.end(), log);
    std::clog << "]" << std::endl;

    int ll = left.size() - 1;
    int left_count (0);
    for ( iterator current = start ; current != end;  )
    {
        if ( left.front() <= right.front() )
        {
            *(current++) = left.front();
            left.pop_front();
            left_count++;
        }
        else
        {
            *(current++) = right.front();
            right.pop_front();
            swaps_ += ll - left_count;
        }
    }
    std::clog << "merged [ ";
    std::copy (start, end, log);
    std::clog << "]" << std::endl;
}

}


u10810::u10810()
{
}

void u10810::operator()()
{
    int n;
    while (std::cin >> n && n > 0)
    {
        sorter s(n);
        std::cin >> s;
        std::cout << s.sort() << std::endl;
    }
}
