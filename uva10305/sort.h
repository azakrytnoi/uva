#pragma once

#include <cstdint>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

namespace util
{
/** \brief utility class, counts number of swaps during sort
 *
 */
template<uint64_t MAX>
class sort
{
public:
    /** \brief Constructor
     *
     * \param n number of elements
     */
    explicit sort(int n) : n_(n), swaps_(0)
    {
    }

    ~sort()
    {
    }

    uint64_t do_sort();

    friend std::istream& operator >> (std::istream& in, sort<MAX>& s)
    {
        int n = s.n_;
        std::istream_iterator<int> iin(in);
        std::copy_n(iin, n, std::back_inserter(s.src_));
        return in;
    }


private:
    typedef std::list<int>::iterator iterator;
    int n_;
    uint64_t swaps_;
    std::list<int> src_;

    void merge_sort(iterator start, iterator end);
    void merge(iterator start, iterator middle, iterator end);
};

template<uint64_t MAX>
inline uint64_t sort<MAX>::do_sort()
{
    merge_sort(src_.begin(), src_.end());
    return swaps_;
}

template<uint64_t MAX>
inline void sort<MAX>::merge_sort(iterator start, iterator end)
{
    if (std::distance(start, end) > 1)
    {
        iterator middle(start);
        std::advance(middle, std::distance(start, end) / 2);
        merge_sort(start, middle);
        merge_sort(middle, end);
        merge(start, middle, end);
    }
}

template<uint64_t MAX>
inline void sort<MAX>::merge(iterator start, iterator middle, iterator end)
{
    std::list <int> left(start, middle);
    std::list <int> right(middle, end);

    left.push_back(MAX + 2);
    right.push_back(MAX + 2);

    int ll = left.size() - 1;
    int left_count(0);
    for (iterator current = start; current != end; )
    {
        if (left.front() <= right.front())
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
}

}
