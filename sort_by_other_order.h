#ifndef _sort_by_other_order_h
#define _sort_by_other_order_h

#include <algorithm>

// This solution works with GCC 5.3.0, Apple's clang-703.0.31 & MS Visual Studio 2015

template <class T1, class T2>
struct ReferencePair;

// Iterator which iterates two sequences simultaneously
template <class Iterator1, class Iterator2>
class DoubleIterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename Iterator1::difference_type;
    using value_type = std::pair<typename Iterator1::value_type, typename Iterator2::value_type>;
    using pointer = std::pair<typename Iterator1::pointer, typename Iterator2::pointer>;
    using reference = ReferencePair<typename Iterator1::value_type, typename Iterator2::value_type>;

    DoubleIterator(Iterator1 it1, Iterator2 it2) : _it1(it1), _it2(it2) {}

    DoubleIterator& operator+=(difference_type n)
    {
        _it1 += n;
        _it2 += n;
        return *this;
    }

    DoubleIterator& operator-=(difference_type n)
    {
        *this += -n;
        return *this;
    }

    difference_type operator-(const DoubleIterator& other) const
    {
        return _it1 - other._it1;
    }

    bool operator==(const DoubleIterator& other) const
    {
        return _it1 == other._it1;
    }

    bool operator!=(const DoubleIterator& other) const
    {
        return !(*this == other);
    }

    bool operator<(const DoubleIterator& other) const
    {
        return _it1 < other._it1;
    }

    bool operator>(const DoubleIterator& other) const
    {
        return other < *this;
    }

    bool operator<=(const DoubleIterator& other) const
    {
        return !(*this > other);
    }

    bool operator>=(const DoubleIterator& other) const
    {
        return !(*this < other);
    }

    reference operator*()
    {
        return reference(*_it1, *_it2);
    }

    DoubleIterator& operator++()
    {
        ++_it1;
        ++_it2;
        return *this;
    }

    DoubleIterator operator++(int)
    {
        DoubleIterator t(*this);
        ++(*this);
        return t;
    }

    DoubleIterator& operator--()
    {
        --_it1;
        --_it2;
        return *this;
    }

    DoubleIterator operator--(int)
    {
        DoubleIterator t(*this);
        --(*this);
        return t;
    }

private:
    Iterator1 _it1;
    Iterator2 _it2;
};

template <class Iterator1, class Iterator2>
DoubleIterator<Iterator1, Iterator2> operator+(const DoubleIterator<Iterator1, Iterator2>& it,
    typename DoubleIterator<Iterator1, Iterator2>::difference_type n)
{
    DoubleIterator<Iterator1, Iterator2> result(it);
    result += n;
    return result;
}

template <class Iterator1, class Iterator2>
DoubleIterator<Iterator1, Iterator2> operator-(const DoubleIterator<Iterator1, Iterator2>& it,
    typename DoubleIterator<Iterator1, Iterator2>::difference_type n)
{
    DoubleIterator<Iterator1, Iterator2> result(it);
    result -= n;
    return result;
}

template <class Iterator1, class Iterator2>
DoubleIterator<Iterator1, Iterator2> makeDoubleIterator(Iterator1 it1, Iterator2 it2)
{
    return DoubleIterator<Iterator1, Iterator2>(it1, it2);
}


// We need to create our own class for the dereferenced iterator to be able to overload std::swap with it
// That is, to be able to do std::swap(*it1, *it2) - this is ValueSwappable requirement to iterators passed to std::sort
template <class T1, class T2>
struct ReferencePair
{
    ReferencePair(T1& first, T2& second) : _first(first), _second(second) {}

    // The type of dereferenced iterator must meet MoveConstructible requirement to be used in std::sort
    ReferencePair(ReferencePair&& other) : _first(other._first), _second(other._second) {}

    // The type of dereferenced iterator must meet MoveAssignable requirement to be used in std::sort
    ReferencePair& operator=(ReferencePair&& other)
    {
        _first = std::move(other._first);
        _second = std::move(other._second);
        return *this;
    }

    // converting iterator's reference type to value type
    operator std::pair<T1, T2>() const
    {
        return std::make_pair(_first, _second);
    }

    // assigning value type to dereferenced iterator
    ReferencePair& operator=(std::pair<T1, T2>&& pair)
    {
        _first = std::move(pair.first);
        _second = std::move(pair.second);
        return *this;
    }

    T1& _first;
    T2& _second;
};

template <class T1, class T2>
void swap(ReferencePair<T1, T2> p1, ReferencePair<T1, T2> p2)
{
    std::swap(p1._first, p2._first);
    std::swap(p1._second, p2._second);
}

template <class Container1, class Container2, class Comparator = std::less<typename Container2::value_type>>
void sortByOtherOrder(Container1& container1, Container2& container2, Comparator comparator = Comparator())
{
    using ValuePair = std::pair<typename Container1::value_type, typename Container2::value_type>;
    auto compareBySecond = [comparator](const ValuePair& p1, const ValuePair& p2)
    {
        return comparator(p1.second, p2.second);
    };

    std::sort(makeDoubleIterator(container1.begin(), container2.begin()),
        makeDoubleIterator(container1.end(), container2.end()),
        compareBySecond);
}

#endif
