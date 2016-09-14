#include "sort_by_other_order.h"

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct A
{
    int x, y;
};

std::ostream& operator<<(std::ostream& o, const A& a)
{
    o << "a(" << a.x << "," << a.y << ")";
    return o;
}

struct B
{
    int a, b;
};

std::ostream& operator<<(std::ostream& o, const B& b)
{
    o << "b(" << b.a << "," << b.b << ")";
    return o;
}

bool bLess(const B& b1, const B& b2)
{
    return b1.a < b2.a;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> c1{ "a", "b", "c", "d", "e", "f", "g", "h", "i", "k" };
    std::vector<int> c2{ 4, 7, 1, 3, 5, 8, 9, 2, 0, 6 };

    sortByOtherOrder(c1, c2);

    std::copy(c1.begin(), c1.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << "\n";
    std::copy(c2.begin(), c2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    std::vector<A> ca{ A{1, 1}, A{ 2, 2 }, A{ 3, 3 } };
    std::vector<B> cb{ B{ 3, 1 }, B{ 1, 2 }, B{ 2, 3 } };

    sortByOtherOrder(ca, cb, bLess);

    std::copy(ca.begin(), ca.end(), std::ostream_iterator<A>(std::cout, " "));
    std::cout << "\n";
    std::copy(cb.begin(), cb.end(), std::ostream_iterator<B>(std::cout, " "));
    std::cout << "\n";

    return 0;
}