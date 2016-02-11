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

    void merge_sort (iterator p, iterator r);
    void merge (iterator p, iterator q, iterator r);
};

sorter::sorter(int n) : n_(n), swaps_(0)
{
}

uint64_t sorter::sort()
{
    merge_sort(src_.begin(), src_.end());
    return swaps_;
}

void sorter::merge_sort (iterator p, iterator r)
{
    std::clog << "merge-sort [ ";
    std::copy(p, r, log);
    std::clog << "]" << std::endl;
    if ( std::distance(p, r) > 1 )
    {
        iterator q ( p);
        std::advance(q, std::distance(p, r) / 2);
        merge_sort (p, q);
        merge_sort (q, r);
        merge (p, q, r);
    std::clog << "merge-sort after [ ";
    std::copy(p, r, log);
    std::clog << "]" << std::endl;
    }
}

void sorter::merge (iterator p, iterator q, iterator r)
{
    std::clog << "merge [ ";
    std::copy(p, q, log);
    std::clog << "/ ";
    std::copy(q, r, log);
    std::clog << "]" << std::endl;
    int len1 = std::distance( p, q) + 1;
    std::list <int> left;
    std::list <int> right;

    std::copy(p, q, std::back_inserter(left));
    std::copy(q, r, std::back_inserter(right));
 //   left.splice(left.begin(), src_, p, q);
 //   right.splice(right.begin(), src_, q, r);

    left.push_back (999999999 + 10);
    right.push_back (999999999 + 10);
    std::clog << "\tleft [ ";
    std::copy (left.begin(), left.end(), log);
    std::clog << "]" << std::endl;
    std::clog << "\tright [ ";
    std::copy (right.begin(), right.end(), log);
    std::clog << "]" << std::endl;

    for ( iterator k = p ; k != r;  )
    {
        if ( left.front() <= right.front() )
        {
            *(k++) = left.front();
            left.pop_front();
        }
        else
        {
            *(k++) = right.front();
            right.pop_front();
            swaps_ += len1 - std::distance(p, k);
        }
    }
    std::clog << "merged [ ";
    std::copy (p, r, log);
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
